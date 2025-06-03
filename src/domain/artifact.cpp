// filepath: src/domain/artifact.cpp
#include "artifact.h"

ArcheologicalArtifact::ArcheologicalArtifact() {}

ArcheologicalArtifact::ArcheologicalArtifact(const QString& id, const QString& name, const QString& description,
                                             const QString& material, const QDate& discoveryDate, const QString& location)
    : m_id(id), m_name(name), m_description(description), m_material(material),
      m_discoveryDate(discoveryDate), m_location(location) {}

QString ArcheologicalArtifact::getId() const { return m_id; }
void ArcheologicalArtifact::setId(const QString& id) { m_id = id; }

QString ArcheologicalArtifact::getName() const { return m_name; }
void ArcheologicalArtifact::setName(const QString& name) { m_name = name; }

QString ArcheologicalArtifact::getDescription() const { return m_description; }
void ArcheologicalArtifact::setDescription(const QString& description) { m_description = description; }

QString ArcheologicalArtifact::getMaterial() const { return m_material; }
void ArcheologicalArtifact::setMaterial(const QString& material) { m_material = material; }

QDate ArcheologicalArtifact::getDiscoveryDate() const { return m_discoveryDate; }
void ArcheologicalArtifact::setDiscoveryDate(const QDate& date) { m_discoveryDate = date; }

QString ArcheologicalArtifact::getLocation() const { return m_location; }
void ArcheologicalArtifact::setLocation(const QString& location) { m_location = location; }