// filepath: src/repository/repository.h
#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <memory> // For std::unique_ptr if needed for return types, or smart pointers in implementations
#include <QString>
#include "../domain/artifact.h" // Path to your ArcheologicalArtifact header

class Repository {
public:
    virtual ~Repository() = default;

    virtual void addArtifact(const ArcheologicalArtifact& artifact) = 0;
    virtual void removeArtifact(const QString& artifactId) = 0;
    virtual void updateArtifact(const ArcheologicalArtifact& artifact) = 0;
    virtual ArcheologicalArtifact findArtifactById(const QString& artifactId) const = 0; // Consider returning optional or throwing if not found
    virtual std::vector<ArcheologicalArtifact> getAllArtifacts() const = 0;
    // You might also need methods like:
    // virtual bool artifactExists(const QString& artifactId) const = 0;
};

#endif // REPOSITORY_H