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
        // throw std::invalid_argument("Artifact ID and Name cannot be empty.");
        // Or handle error via return type/status
        return;
    }
    ArcheologicalArtifact artifact(id, name, description, material, discoveryDate, location);
    // Consider checking for duplicate IDs before adding if IDs must be unique
    // if (m_repository->artifactExists(id)) {
    //     throw std::runtime_error("Artifact with this ID already exists.");
    // }
    m_repository->addArtifact(artifact);
    // TODO: Implement Command pattern: Create and execute AddArtifactCommand
    // m_undoStack.clear(); // A new action typically clears the redo stack
}

void ArtifactController::removeArtifact(const QString& artifactId) {
    if (artifactId.isEmpty()) {
        // throw std::invalid_argument("Artifact ID cannot be empty for removal.");
        return;
    }
    // TODO: Check if artifact exists before attempting removal to provide better feedback or avoid errors
    m_repository->removeArtifact(artifactId);
    // TODO: Implement Command pattern: Create and execute RemoveArtifactCommand
    // m_undoStack.clear();
}

void ArtifactController::updateArtifact(const QString& originalId, const QString& newId, const QString& name, const QString& description,
                                        const QString& material, const QDate& discoveryDate, const QString& location) {
    if (originalId.isEmpty() || newId.isEmpty() || name.isEmpty()) {
        // throw std::invalid_argument("Artifact IDs and Name cannot be empty for update.");
        return;
    }
    ArcheologicalArtifact updatedArtifact(newId, name, description, material, discoveryDate, location);
    // If ID can change, the repository needs to handle this (e.g., remove old, add new, or have a specific update method)
    // For simplicity, assuming updateArtifact in repository can handle ID changes or that originalId is used to find and then update.
    // If the ID is part of the update, the repository's updateArtifact method needs to know the original ID.
    // A common approach is for updateArtifact to take the full new object, and it uses its ID to find and replace.
    // If the ID itself can be changed, you might need:
    // m_repository->removeArtifact(originalId);
    // m_repository->addArtifact(updatedArtifact);
    // Or, the repository's updateArtifact method takes originalId and the new artifact data.
    // For now, let's assume the repository's updateArtifact method finds by the ID within updatedArtifact.
    m_repository->updateArtifact(updatedArtifact); // This implies the repository finds the artifact by updatedArtifact.getId()
                                                   // and updates it. If the ID can change, this needs careful handling.
    // TODO: Implement Command pattern: Create and execute UpdateArtifactCommand
    // m_undoStack.clear();
}

ArcheologicalArtifact ArtifactController::getArtifactById(const QString& artifactId) const {
    if (artifactId.isEmpty()) {
        // throw std::invalid_argument("Artifact ID cannot be empty for search.");
        // Or return a "null" or optional artifact
    }
    return m_repository->findArtifactById(artifactId); // Repository should handle not found (e.g. throw, return default/optional)
}

std::vector<ArcheologicalArtifact> ArtifactController::getAllArtifacts() const {
    return m_repository->getAllArtifacts();
}

// TODO: Implement methods for filtering, undo, redo
// Example for undo/redo command execution:
// void ArtifactController::executeCommand(std::unique_ptr<Command> command) {
//     command->execute();
//     m_undoStack.push_back(std::move(command));
//     m_redoStack.clear(); // New commands clear the redo stack
// }
//
// bool ArtifactController::canUndo() const { return !m_undoStack.empty(); }
// void ArtifactController::undo() {
//     if (canUndo()) {
//         std::unique_ptr<Command> command = std::move(m_undoStack.back());
//         m_undoStack.pop_back();
//         command->undo();
//         m_redoStack.push_back(std::move(command));
//     }
// }
// bool ArtifactController::canRedo() const { return !m_redoStack.empty(); }
// void ArtifactController::redo() {
//     if (canRedo()) {
//         std::unique_ptr<Command> command = std::move(m_redoStack.back());
//         m_redoStack.pop_back();
//         command->execute(); // Or a specific redo() if different
//         m_undoStack.push_back(std::move(command));
//     }
// }