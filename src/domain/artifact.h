#ifndef ARTIFACT_H
#define ARTIFACT_H

#include <QString>
#include <QDate> // For discovery date

class ArcheologicalArtifact {
public:
    ArcheologicalArtifact();
    ArcheologicalArtifact(const QString& id,
                          const QString& name,
                          const QString& description,
                          const QString& material,
                          const QDate& discoveryDate,
                          const QString& location);

    QString getId() const;
    void setId(const QString& id);

    QString getName() const;
    void setName(const QString& name);

    QString getDescription() const;
    void setDescription(const QString& description);

    QString getMaterial() const;
    void setMaterial(const QString& material);

    QDate getDiscoveryDate() const;
    void setDiscoveryDate(const QDate& date);

    QString getLocation() const;
    void setLocation(const QString& location);

    // Add other properties and their getters/setters as needed
    // For example: QString getPhotoPath() const; void setPhotoPath(const QString& path);

private:
    QString m_id;
    QString m_name;
    QString m_description;
    QString m_material;
    QDate m_discoveryDate;
    QString m_location;
    // QString m_photoPath;
};

#endif // ARTIFACT_H