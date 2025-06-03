// filepath: src/controller/artifact_controller.h
#ifndef ARTIFACT_CONTROLLER_H
#define ARTIFACT_CONTROLLER_H

#include "../repository/repository.h"
#include "../domain/artifact.h"
#include <vector>
#include <memory> // For std::unique_ptr to hold the repository
#include <QString>
#include <QDate>

// Forward declarations for Command pattern classes if they are defined elsewhere
// class Command;
// class AddArtifactCommand;
// class RemoveArtifactCommand;
// class UpdateArtifactCommand;

class ArtifactController {
public:
    // The controller will take ownership of the repository
    ArtifactController(std::unique_ptr<Repository> repo);
    ~ArtifactController();

    void addArtifact(const QString& id, const QString& name, const QString& description,
                     const QString& material, const QDate& discoveryDate, const QString& location);
    void removeArtifact(const QString& artifactId);
    void updateArtifact(const QString& originalId, const QString& newId, const QString& name, const QString& description,
                        const QString& material, const QDate& discoveryDate, const QString& location);

    ArcheologicalArtifact getArtifactById(const QString& artifactId) const;
    std::vector<ArcheologicalArtifact> getAllArtifacts() const;

    // TODO: Implement filtering functionality
    // std::vector<ArcheologicalArtifact> filterArtifactsByName(const QString& name) const;
    // std::vector<ArcheologicalArtifact> filterArtifactsByMaterial(const QString& material) const;
    // ... other filter methods or a more generic filter approach

    // TODO: Implement Undo/Redo functionality
    // bool canUndo() const;
    // bool canRedo() const;
    // void undo();
    // void redo();

private:
    std::unique_ptr<Repository> m_repository;

    // For Undo/Redo stack:
    // std::vector<std::unique_ptr<Command>> m_undoStack;
    // std::vector<std::unique_ptr<Command>> m_redoStack;
    // void executeCommand(std::unique_ptr<Command> command);
};

#endif // ARTIFACT_CONTROLLER_H