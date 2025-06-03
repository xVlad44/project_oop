// filepath: src/controller/artifact_controller.h
#ifndef ARTIFACT_CONTROLLER_H
#define ARTIFACT_CONTROLLER_H

#include "../repository/repository.h"
#include "../domain/artifact.h"
#include "command.h"
#include "filter.h"
#include <vector>
#include <memory> // For std::unique_ptr to hold the repository
#include <stack>
#include <QString>
#include <QDate>

class ArtifactController {
public:
    // The controller will take ownership of the repository
    ArtifactController(std::unique_ptr<Repository> repo);
    ~ArtifactController();

    void addArtifact(const QString& id, const QString& name, const QString& description,
                     const QString& material, const QDate& discoveryDate, const QString& location);
    void removeArtifact(const QString& artifactId);    void updateArtifact(const QString& originalId, const QString& newId, const QString& name, const QString& description,
                        const QString& material, const QDate& discoveryDate, const QString& location);
    
    ArcheologicalArtifact getArtifactById(const QString& artifactId) const;
    std::vector<ArcheologicalArtifact> getAllArtifacts() const;

    // Filtering functionality
    std::vector<ArcheologicalArtifact> filterArtifacts(std::unique_ptr<FilterStrategy> filter) const;
    std::vector<ArcheologicalArtifact> filterArtifactsByName(const QString& name, bool caseSensitive = false) const;
    std::vector<ArcheologicalArtifact> filterArtifactsByMaterial(const QString& material, bool caseSensitive = false) const;
    std::vector<ArcheologicalArtifact> filterArtifactsByLocation(const QString& location, bool caseSensitive = false) const;
    std::vector<ArcheologicalArtifact> filterArtifactsByDateRange(const QDate& startDate, const QDate& endDate) const;

    // Undo/Redo functionality
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;    void clearHistory();

private:
    std::unique_ptr<Repository> m_repository;
    
    // Command pattern for undo/redo
    std::stack<std::unique_ptr<Command>> m_undoStack;
    std::stack<std::unique_ptr<Command>> m_redoStack;
    
    void executeCommand(std::unique_ptr<Command> command);
};

#endif // ARTIFACT_CONTROLLER_H