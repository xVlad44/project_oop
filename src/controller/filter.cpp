#include "filter.h"

// NameFilter Implementation
NameFilter::NameFilter(const QString& name, bool caseSensitive)
    : m_name(name), m_caseSensitive(caseSensitive) {}

bool NameFilter::matches(const ArcheologicalArtifact& artifact) const {
    if (m_caseSensitive) {
        return artifact.getName().contains(m_name);
    } else {
        return artifact.getName().contains(m_name, Qt::CaseInsensitive);
    }
}

std::unique_ptr<FilterStrategy> NameFilter::clone() const {
    return std::make_unique<NameFilter>(m_name, m_caseSensitive);
}

// MaterialFilter Implementation
MaterialFilter::MaterialFilter(const QString& material, bool caseSensitive)
    : m_material(material), m_caseSensitive(caseSensitive) {}

bool MaterialFilter::matches(const ArcheologicalArtifact& artifact) const {
    if (m_caseSensitive) {
        return artifact.getMaterial().contains(m_material);
    } else {
        return artifact.getMaterial().contains(m_material, Qt::CaseInsensitive);
    }
}

std::unique_ptr<FilterStrategy> MaterialFilter::clone() const {
    return std::make_unique<MaterialFilter>(m_material, m_caseSensitive);
}

// LocationFilter Implementation
LocationFilter::LocationFilter(const QString& location, bool caseSensitive)
    : m_location(location), m_caseSensitive(caseSensitive) {}

bool LocationFilter::matches(const ArcheologicalArtifact& artifact) const {
    if (m_caseSensitive) {
        return artifact.getLocation().contains(m_location);
    } else {
        return artifact.getLocation().contains(m_location, Qt::CaseInsensitive);
    }
}

std::unique_ptr<FilterStrategy> LocationFilter::clone() const {
    return std::make_unique<LocationFilter>(m_location, m_caseSensitive);
}

// DateRangeFilter Implementation
DateRangeFilter::DateRangeFilter(const QDate& startDate, const QDate& endDate)
    : m_startDate(startDate), m_endDate(endDate) {}

bool DateRangeFilter::matches(const ArcheologicalArtifact& artifact) const {
    QDate discoveryDate = artifact.getDiscoveryDate();
    return discoveryDate >= m_startDate && discoveryDate <= m_endDate;
}

std::unique_ptr<FilterStrategy> DateRangeFilter::clone() const {
    return std::make_unique<DateRangeFilter>(m_startDate, m_endDate);
}

// IdFilter Implementation
IdFilter::IdFilter(const QString& id, bool caseSensitive)
    : m_id(id), m_caseSensitive(caseSensitive) {}

bool IdFilter::matches(const ArcheologicalArtifact& artifact) const {
    if (m_caseSensitive) {
        return artifact.getId().contains(m_id);
    } else {
        return artifact.getId().contains(m_id, Qt::CaseInsensitive);
    }
}

std::unique_ptr<FilterStrategy> IdFilter::clone() const {
    return std::make_unique<IdFilter>(m_id, m_caseSensitive);
}

// AndFilter Implementation
void AndFilter::addFilter(std::unique_ptr<FilterStrategy> filter) {
    m_filters.push_back(std::move(filter));
}

bool AndFilter::matches(const ArcheologicalArtifact& artifact) const {
    for (const auto& filter : m_filters) {
        if (!filter->matches(artifact)) {
            return false;
        }
    }
    return !m_filters.empty(); // Return false if no filters are added
}

std::unique_ptr<FilterStrategy> AndFilter::clone() const {
    auto clone = std::make_unique<AndFilter>();
    for (const auto& filter : m_filters) {
        clone->addFilter(filter->clone());
    }
    return clone;
}

// OrFilter Implementation
void OrFilter::addFilter(std::unique_ptr<FilterStrategy> filter) {
    m_filters.push_back(std::move(filter));
}

bool OrFilter::matches(const ArcheologicalArtifact& artifact) const {
    for (const auto& filter : m_filters) {
        if (filter->matches(artifact)) {
            return true;
        }
    }
    return false; // Return false if no filters match or no filters are added
}

std::unique_ptr<FilterStrategy> OrFilter::clone() const {
    auto clone = std::make_unique<OrFilter>();
    for (const auto& filter : m_filters) {
        clone->addFilter(filter->clone());
    }
    return clone;
}

// ArtifactFilter Implementation
ArtifactFilter::ArtifactFilter(std::unique_ptr<FilterStrategy> strategy)
    : m_strategy(std::move(strategy)) {}

void ArtifactFilter::setStrategy(std::unique_ptr<FilterStrategy> strategy) {
    m_strategy = std::move(strategy);
}

std::vector<ArcheologicalArtifact> ArtifactFilter::filter(const std::vector<ArcheologicalArtifact>& artifacts) const {
    if (!m_strategy) {
        return artifacts; // Return all if no strategy is set
    }
    
    std::vector<ArcheologicalArtifact> result;
    for (const auto& artifact : artifacts) {
        if (m_strategy->matches(artifact)) {
            result.push_back(artifact);
        }
    }
    
    return result;
}
