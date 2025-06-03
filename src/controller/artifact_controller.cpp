// filepath: src/controller/artifact_controller.cpp
#include "artifact_controller.h"
#include <stdexcept> // For std::runtime_error or specific exceptions

ArtifactController::ArtifactController(std::unique_ptr<Repository> repo)
    : m_repository(std::move(repo)) {
    if (!m_repository) {
        // Consider throwing a more specific exception or logging
        throw std::runtime_error("Repository provided to ArtifactController is null.");
    }
}

ArtifactController::~ArtifactController() = default; // Default destructor is fine with unique_ptr

void ArtifactController::addArtifact(const QString& id, const QString& name, const QString& description,
                                     const QString& material, const QDate& discoveryDate, const QString& location) {
    // Basic validation (can be expanded)
    if (id.isEmpty() || name.isEmpty()) {
        throw std::invalid_argument("Artifact ID and Name cannot be empty.");
    }
    
    ArcheologicalArtifact artifact(id, name, description, material, discoveryDate, location);
    auto command = std::make_unique<AddArtifactCommand>(m_repository.get(), artifact);
    executeCommand(std::move(command));
}

void ArtifactController::removeArtifact(const QString& artifactId) {
    if (artifactId.isEmpty()) {
        throw std::invalid_argument("Artifact ID cannot be empty for removal.");
    }
    
    auto command = std::make_unique<RemoveArtifactCommand>(m_repository.get(), artifactId);
    executeCommand(std::move(command));
}

void ArtifactController::updateArtifact(const QString& originalId, const QString& newId, const QString& name, const QString& description,
                                        const QString& material, const QDate& discoveryDate, const QString& location) {
    if (originalId.isEmpty() || newId.isEmpty() || name.isEmpty()) {
        throw std::invalid_argument("Artifact IDs and Name cannot be empty for update.");
    }
    
    ArcheologicalArtifact updatedArtifact(newId, name, description, material, discoveryDate, location);
    
    // Handle ID changes by removing old and adding new if IDs are different
    if (originalId != newId) {
        // For ID changes, we need to ensure the original artifact exists first
        m_repository->findArtifactById(originalId); // This will throw if not found
        removeArtifact(originalId);
        addArtifact(newId, name, description, material, discoveryDate, location);
    } else {
        auto command = std::make_unique<UpdateArtifactCommand>(m_repository.get(), updatedArtifact);
        executeCommand(std::move(command));
    }
}

ArcheologicalArtifact ArtifactController::getArtifactById(const QString& artifactId) const {
    if (artifactId.isEmpty()) {
        throw std::invalid_argument("Artifact ID cannot be empty for search.");
    }
    return m_repository->findArtifactById(artifactId);
}

std::vector<ArcheologicalArtifact> ArtifactController::getAllArtifacts() const {
    return m_repository->getAllArtifacts();
}

// Filtering functionality
std::vector<ArcheologicalArtifact> ArtifactController::filterArtifacts(std::unique_ptr<FilterStrategy> filter) const {
    ArtifactFilter artifactFilter(std::move(filter));
    return artifactFilter.filter(getAllArtifacts());
}

std::vector<ArcheologicalArtifact> ArtifactController::filterArtifactsByName(const QString& name, bool caseSensitive) const {
    auto filter = std::make_unique<NameFilter>(name, caseSensitive);
    return filterArtifacts(std::move(filter));
}

std::vector<ArcheologicalArtifact> ArtifactController::filterArtifactsByMaterial(const QString& material, bool caseSensitive) const {
    auto filter = std::make_unique<MaterialFilter>(material, caseSensitive);
    return filterArtifacts(std::move(filter));
}

std::vector<ArcheologicalArtifact> ArtifactController::filterArtifactsByLocation(const QString& location, bool caseSensitive) const {
    auto filter = std::make_unique<LocationFilter>(location, caseSensitive);
    return filterArtifacts(std::move(filter));
}

std::vector<ArcheologicalArtifact> ArtifactController::filterArtifactsByDateRange(const QDate& startDate, const QDate& endDate) const {
    auto filter = std::make_unique<DateRangeFilter>(startDate, endDate);
    return filterArtifacts(std::move(filter));
}

// Undo/Redo functionality
void ArtifactController::executeCommand(std::unique_ptr<Command> command) {
    command->execute();
    m_undoStack.push(std::move(command));
    
    // Clear redo stack when a new command is executed
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }
}

bool ArtifactController::canUndo() const {
    return !m_undoStack.empty();
}

void ArtifactController::undo() {
    if (canUndo()) {
        auto command = std::move(const_cast<std::unique_ptr<Command>&>(m_undoStack.top()));
        m_undoStack.pop();
        command->undo();
        m_redoStack.push(std::move(command));
    }
}

bool ArtifactController::canRedo() const {
    return !m_redoStack.empty();
}

void ArtifactController::redo() {
    if (canRedo()) {
        auto command = std::move(const_cast<std::unique_ptr<Command>&>(m_redoStack.top()));
        m_redoStack.pop();
        command->execute();
        m_undoStack.push(std::move(command));
    }
}

void ArtifactController::clearHistory() {
    while (!m_undoStack.empty()) {
        m_undoStack.pop();
    }
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }
}
