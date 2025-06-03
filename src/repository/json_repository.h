#ifndef JSON_REPOSITORY_H
#define JSON_REPOSITORY_H

#include "repository.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JsonRepository : public Repository {
public:
    explicit JsonRepository(const QString& filePath);
    ~JsonRepository() override = default;

    void addArtifact(const ArcheologicalArtifact& artifact) override;
    void removeArtifact(const QString& artifactId) override;
    void updateArtifact(const ArcheologicalArtifact& artifact) override;
    ArcheologicalArtifact findArtifactById(const QString& artifactId) const override;
    std::vector<ArcheologicalArtifact> getAllArtifacts() const override;

private:
    QString m_filePath;
    
    void loadFromFile();
    void saveToFile() const;
    QJsonObject artifactToJson(const ArcheologicalArtifact& artifact) const;
    ArcheologicalArtifact jsonToArtifact(const QJsonObject& jsonObj) const;
    
    mutable std::vector<ArcheologicalArtifact> m_artifacts; // Cache for loaded artifacts
    mutable bool m_loaded = false;
};

#endif // JSON_REPOSITORY_H
