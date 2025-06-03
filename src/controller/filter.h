#ifndef FILTER_H
#define FILTER_H

#include "../domain/artifact.h"
#include <vector>
#include <memory>
#include <QString>
#include <QDate>

// Strategy interface for filtering
class FilterStrategy {
public:
    virtual ~FilterStrategy() = default;
    virtual bool matches(const ArcheologicalArtifact& artifact) const = 0;
    virtual std::unique_ptr<FilterStrategy> clone() const = 0;
};

// Concrete filter strategies
class NameFilter : public FilterStrategy {
public:
    explicit NameFilter(const QString& name, bool caseSensitive = false);
    bool matches(const ArcheologicalArtifact& artifact) const override;
    std::unique_ptr<FilterStrategy> clone() const override;

private:
    QString m_name;
    bool m_caseSensitive;
};

class MaterialFilter : public FilterStrategy {
public:
    explicit MaterialFilter(const QString& material, bool caseSensitive = false);
    bool matches(const ArcheologicalArtifact& artifact) const override;
    std::unique_ptr<FilterStrategy> clone() const override;

private:
    QString m_material;
    bool m_caseSensitive;
};

class LocationFilter : public FilterStrategy {
public:
    explicit LocationFilter(const QString& location, bool caseSensitive = false);
    bool matches(const ArcheologicalArtifact& artifact) const override;
    std::unique_ptr<FilterStrategy> clone() const override;

private:
    QString m_location;
    bool m_caseSensitive;
};

class DateRangeFilter : public FilterStrategy {
public:
    DateRangeFilter(const QDate& startDate, const QDate& endDate);
    bool matches(const ArcheologicalArtifact& artifact) const override;
    std::unique_ptr<FilterStrategy> clone() const override;

private:
    QDate m_startDate;
    QDate m_endDate;
};

class IdFilter : public FilterStrategy {
public:
    explicit IdFilter(const QString& id, bool caseSensitive = false);
    bool matches(const ArcheologicalArtifact& artifact) const override;
    std::unique_ptr<FilterStrategy> clone() const override;

private:
    QString m_id;
    bool m_caseSensitive;
};

// Composite filters for AND/OR operations
class AndFilter : public FilterStrategy {
public:
    void addFilter(std::unique_ptr<FilterStrategy> filter);
    bool matches(const ArcheologicalArtifact& artifact) const override;
    std::unique_ptr<FilterStrategy> clone() const override;

private:
    std::vector<std::unique_ptr<FilterStrategy>> m_filters;
};

class OrFilter : public FilterStrategy {
public:
    void addFilter(std::unique_ptr<FilterStrategy> filter);
    bool matches(const ArcheologicalArtifact& artifact) const override;
    std::unique_ptr<FilterStrategy> clone() const override;

private:
    std::vector<std::unique_ptr<FilterStrategy>> m_filters;
};

// Filter context that applies the strategy
class ArtifactFilter {
public:
    ArtifactFilter() = default;
    explicit ArtifactFilter(std::unique_ptr<FilterStrategy> strategy);
    
    void setStrategy(std::unique_ptr<FilterStrategy> strategy);
    std::vector<ArcheologicalArtifact> filter(const std::vector<ArcheologicalArtifact>& artifacts) const;

private:
    std::unique_ptr<FilterStrategy> m_strategy;
};

#endif // FILTER_H
