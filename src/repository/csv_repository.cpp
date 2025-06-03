#include "csv_repository.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDate>
#include <QDebug>
#include <stdexcept>

CsvRepository::CsvRepository(const QString& filePath) : m_filePath(filePath) {
    loadFromFile();
}

void CsvRepository::addArtifact(const ArcheologicalArtifact& artifact) {
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

void CsvRepository::removeArtifact(const QString& artifactId) {
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

void CsvRepository::updateArtifact(const ArcheologicalArtifact& artifact) {
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

ArcheologicalArtifact CsvRepository::findArtifactById(const QString& artifactId) const {
    loadFromFile(); // This call will now be valid
    
    for (const auto& artifact : m_artifacts) {
        if (artifact.getId() == artifactId) {
            return artifact;
        }
    }
    
    throw std::runtime_error("Artifact with ID '" + artifactId.toStdString() + "' not found.");
}

std::vector<ArcheologicalArtifact> CsvRepository::getAllArtifacts() const {
    loadFromFile(); // This call will also be valid if loadFromFile is const
    return m_artifacts;
}

void CsvRepository::loadFromFile() const { // Add const here
    if (m_loaded) return;
    
    m_artifacts.clear(); // Modifying mutable member
    
    QFile file(m_filePath);
    if (!file.exists()) {
        // File doesn't exist yet, start with empty repository
        m_loaded = true;
        return;
    }
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Cannot open file for reading: " + m_filePath.toStdString());
    }
    
    QTextStream stream(&file);
    QString headerLine = stream.readLine(); // Skip header line
    
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty()) continue;
        
        try {
            std::vector<QString> fields = parseCSVLine(line);
            if (fields.size() >= 6) {
                QString id = unescapeCSVField(fields[0]);
                QString name = unescapeCSVField(fields[1]);
                QString description = unescapeCSVField(fields[2]);
                QString material = unescapeCSVField(fields[3]);
                QDate discoveryDate = QDate::fromString(unescapeCSVField(fields[4]), Qt::ISODate);
                QString location = unescapeCSVField(fields[5]);
                
                m_artifacts.emplace_back(id, name, description, material, discoveryDate, location);
            }
        } catch (const std::exception& e) {
            qDebug() << "Error parsing CSV line:" << line << " - " << e.what();
        }
    }
    
    file.close();
    m_loaded = true; // Modifying mutable member
}

void CsvRepository::saveToFile() const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("Cannot open file for writing: " + m_filePath.toStdString());
    }
    
    QTextStream stream(&file);
    
    // Write header
    stream << "ID,Name,Description,Material,DiscoveryDate,Location\n";
    
    // Write artifacts
    for (const auto& artifact : m_artifacts) {
        stream << formatCSVLine(artifact) << "\n";
    }
    
    file.close();
}

QString CsvRepository::escapeCSVField(const QString& field) const {
    if (field.contains(',') || field.contains('"') || field.contains('\n') || field.contains('\r')) {
        QString escaped = field;
        escaped.replace("\"", "\"\""); // Escape quotes by doubling them
        return "\"" + escaped + "\"";
    }
    return field;
}

QString CsvRepository::unescapeCSVField(const QString& field) const {
    if (field.startsWith('"') && field.endsWith('"')) {
        QString unescaped = field.mid(1, field.length() - 2);
        unescaped.replace("\"\"", "\""); // Unescape doubled quotes
        return unescaped;
    }
    return field;
}

std::vector<QString> CsvRepository::parseCSVLine(const QString& line) const {
    std::vector<QString> fields;
    QString current;
    bool inQuotes = false;
    
    for (int i = 0; i < line.length(); ++i) {
        QChar c = line[i];
        
        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                // Escaped quote
                current += '"';
                ++i; // Skip next quote
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    
    fields.push_back(current); // Last field
    return fields;
}

QString CsvRepository::formatCSVLine(const ArcheologicalArtifact& artifact) const {
    QStringList fields;
    fields << escapeCSVField(artifact.getId());
    fields << escapeCSVField(artifact.getName());
    fields << escapeCSVField(artifact.getDescription());
    fields << escapeCSVField(artifact.getMaterial());
    fields << escapeCSVField(artifact.getDiscoveryDate().toString(Qt::ISODate));
    fields << escapeCSVField(artifact.getLocation());
    
    return fields.join(",");
}
