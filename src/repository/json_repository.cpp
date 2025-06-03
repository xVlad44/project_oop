#include "json_repository.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <QDebug>
#include <stdexcept>

JsonRepository::JsonRepository(const QString& filePath) : m_filePath(filePath) {
    loadFromFile();
}

void JsonRepository::addArtifact(const ArcheologicalArtifact& artifact) {
    loadFromFile();
    
    // Check for duplicate IDs
    for (const auto& existingArtifact : m_artifacts) {
        if (existingArtifact.getId() == artifact.getId()) {
            throw std::runtime_error("Artifact with ID '" + artifact.getId().toStdString() + "' already exists.");
        }
    }
    
    m_artifacts.push_back(artifact);
    saveToFile();
}

void JsonRepository::removeArtifact(const QString& artifactId) {
    loadFromFile();
    
    auto it = std::remove_if(m_artifacts.begin(), m_artifacts.end(),
                            [&artifactId](const ArcheologicalArtifact& artifact) {
                                return artifact.getId() == artifactId;
                            });
    
    if (it == m_artifacts.end()) {
        throw std::runtime_error("Artifact with ID '" + artifactId.toStdString() + "' not found.");
    }
    
    m_artifacts.erase(it, m_artifacts.end());
    saveToFile();
}

void JsonRepository::updateArtifact(const ArcheologicalArtifact& artifact) {
    loadFromFile();
    
    for (auto& existingArtifact : m_artifacts) {
        if (existingArtifact.getId() == artifact.getId()) {
            existingArtifact = artifact;
            saveToFile();
            return;
        }
    }
    
    throw std::runtime_error("Artifact with ID '" + artifact.getId().toStdString() + "' not found for update.");
}

ArcheologicalArtifact JsonRepository::findArtifactById(const QString& artifactId) const {
    loadFromFile();
    
    for (const auto& artifact : m_artifacts) {
        if (artifact.getId() == artifactId) {
            return artifact;
        }
    }
    
    throw std::runtime_error("Artifact with ID '" + artifactId.toStdString() + "' not found.");
}

std::vector<ArcheologicalArtifact> JsonRepository::getAllArtifacts() const {
    loadFromFile();
    return m_artifacts;
}

void JsonRepository::loadFromFile() {
    if (m_loaded) return;
    
    m_artifacts.clear();
    
    QFile file(m_filePath);
    if (!file.exists()) {
        // File doesn't exist yet, start with empty repository
        m_loaded = true;
        return;
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Cannot open JSON file for reading: " + m_filePath.toStdString());
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        throw std::runtime_error("JSON parse error: " + parseError.errorString().toStdString());
    }
    
    if (!doc.isObject()) {
        throw std::runtime_error("JSON document is not an object");
    }
    
    QJsonObject rootObject = doc.object();
    QJsonArray artifactsArray = rootObject["artifacts"].toArray();
    
    for (const auto& value : artifactsArray) {
        if (value.isObject()) {
            try {
                ArcheologicalArtifact artifact = jsonToArtifact(value.toObject());
                m_artifacts.push_back(artifact);
            } catch (const std::exception& e) {
                qDebug() << "Error parsing JSON artifact:" << e.what();
            }
        }
    }
    
    m_loaded = true;
}

void JsonRepository::saveToFile() const {
    QJsonArray artifactsArray;
    
    for (const auto& artifact : m_artifacts) {
        artifactsArray.append(artifactToJson(artifact));
    }
    
    QJsonObject rootObject;
    rootObject["artifacts"] = artifactsArray;
    rootObject["version"] = "1.0";
    rootObject["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(rootObject);
    
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Cannot open JSON file for writing: " + m_filePath.toStdString());
    }
    
    file.write(doc.toJson());
    file.close();
}

QJsonObject JsonRepository::artifactToJson(const ArcheologicalArtifact& artifact) const {
    QJsonObject obj;
    obj["id"] = artifact.getId();
    obj["name"] = artifact.getName();
    obj["description"] = artifact.getDescription();
    obj["material"] = artifact.getMaterial();
    obj["discoveryDate"] = artifact.getDiscoveryDate().toString(Qt::ISODate);
    obj["location"] = artifact.getLocation();
    return obj;
}

ArcheologicalArtifact JsonRepository::jsonToArtifact(const QJsonObject& jsonObj) const {
    QString id = jsonObj["id"].toString();
    QString name = jsonObj["name"].toString();
    QString description = jsonObj["description"].toString();
    QString material = jsonObj["material"].toString();
    QDate discoveryDate = QDate::fromString(jsonObj["discoveryDate"].toString(), Qt::ISODate);
    QString location = jsonObj["location"].toString();
    
    return ArcheologicalArtifact(id, name, description, material, discoveryDate, location);
}
