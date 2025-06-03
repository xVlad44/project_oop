#ifndef CSV_REPOSITORY_H
#define CSV_REPOSITORY_H

#include "repository.h"
#include <QString>
#include <QTextStream>
#include <QFile>

class CsvRepository : public Repository {
public:
    explicit CsvRepository(const QString& filePath);
    ~CsvRepository() override = default;

    void addArtifact(const ArcheologicalArtifact& artifact) override;
    void removeArtifact(const QString& artifactId) override;
    void updateArtifact(const ArcheologicalArtifact& artifact) override;
    ArcheologicalArtifact findArtifactById(const QString& artifactId) const override;
    std::vector<ArcheologicalArtifact> getAllArtifacts() const override;

private:
    QString m_filePath;
    
    void loadFromFile() const; // Add const here
    void saveToFile() const;
    QString escapeCSVField(const QString& field) const;
    QString unescapeCSVField(const QString& field) const;
    std::vector<QString> parseCSVLine(const QString& line) const;
    QString formatCSVLine(const ArcheologicalArtifact& artifact) const;
    
    mutable std::vector<ArcheologicalArtifact> m_artifacts; // Cache for loaded artifacts
    mutable bool m_loaded = false;
};

#endif // CSV_REPOSITORY_H
