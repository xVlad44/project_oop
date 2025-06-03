#ifndef COMMAND_H
#define COMMAND_H

#include "../repository/repository.h"
#include "../domain/artifact.h"
#include <memory>

// Abstract Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::unique_ptr<Command> clone() const = 0;
};

// Add Artifact Command
class AddArtifactCommand : public Command {
public:
    AddArtifactCommand(Repository* repository, const ArcheologicalArtifact& artifact);
    
    void execute() override;
    void undo() override;
    std::unique_ptr<Command> clone() const override;

private:
    Repository* m_repository;
    ArcheologicalArtifact m_artifact;
};

// Remove Artifact Command
class RemoveArtifactCommand : public Command {
public:
    RemoveArtifactCommand(Repository* repository, const QString& artifactId);
    
    void execute() override;
    void undo() override;
    std::unique_ptr<Command> clone() const override;

private:
    Repository* m_repository;
    QString m_artifactId;
    ArcheologicalArtifact m_removedArtifact; // Store for undo
    bool m_executed = false;
};

// Update Artifact Command
class UpdateArtifactCommand : public Command {
public:
    UpdateArtifactCommand(Repository* repository, const ArcheologicalArtifact& newArtifact);
    
    void execute() override;
    void undo() override;
    std::unique_ptr<Command> clone() const override;

private:
    Repository* m_repository;
    ArcheologicalArtifact m_newArtifact;
    ArcheologicalArtifact m_oldArtifact; // Store for undo
    bool m_executed = false;
};

#endif // COMMAND_H
