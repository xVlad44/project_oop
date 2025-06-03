#include "command.h"
#include <stdexcept>

// AddArtifactCommand Implementation
AddArtifactCommand::AddArtifactCommand(Repository* repository, const ArcheologicalArtifact& artifact)
    : m_repository(repository), m_artifact(artifact) {}

void AddArtifactCommand::execute() {
    m_repository->addArtifact(m_artifact);
}

void AddArtifactCommand::undo() {
    m_repository->removeArtifact(m_artifact.getId());
}

std::unique_ptr<Command> AddArtifactCommand::clone() const {
    return std::make_unique<AddArtifactCommand>(m_repository, m_artifact);
}

// RemoveArtifactCommand Implementation
RemoveArtifactCommand::RemoveArtifactCommand(Repository* repository, const QString& artifactId)
    : m_repository(repository), m_artifactId(artifactId) {}

void RemoveArtifactCommand::execute() {
    if (!m_executed) {
        // Store the artifact before removing it for undo
        m_removedArtifact = m_repository->findArtifactById(m_artifactId);
        m_executed = true;
    }
    m_repository->removeArtifact(m_artifactId);
}

void RemoveArtifactCommand::undo() {
    if (m_executed) {
        m_repository->addArtifact(m_removedArtifact);
    }
}

std::unique_ptr<Command> RemoveArtifactCommand::clone() const {
    auto clone = std::make_unique<RemoveArtifactCommand>(m_repository, m_artifactId);
    if (m_executed) {
        clone->m_removedArtifact = m_removedArtifact;
        clone->m_executed = m_executed;
    }
    return clone;
}

// UpdateArtifactCommand Implementation
UpdateArtifactCommand::UpdateArtifactCommand(Repository* repository, const ArcheologicalArtifact& newArtifact)
    : m_repository(repository), m_newArtifact(newArtifact) {}

void UpdateArtifactCommand::execute() {
    if (!m_executed) {
        // Store the old artifact before updating for undo
        m_oldArtifact = m_repository->findArtifactById(m_newArtifact.getId());
        m_executed = true;
    }
    m_repository->updateArtifact(m_newArtifact);
}

void UpdateArtifactCommand::undo() {
    if (m_executed) {
        m_repository->updateArtifact(m_oldArtifact);
    }
}

std::unique_ptr<Command> UpdateArtifactCommand::clone() const {
    auto clone = std::make_unique<UpdateArtifactCommand>(m_repository, m_newArtifact);
    if (m_executed) {
        clone->m_oldArtifact = m_oldArtifact;
        clone->m_executed = m_executed;
    }
    return clone;
}
