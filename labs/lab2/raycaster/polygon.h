#pragma once
#include <vector>
#include <QPointF>
#include <optional>
#include "ray.h"

class Polygon {
public:
    Polygon() = default;
    Polygon(const std::vector<QPointF>& vertices);

    const std::vector<QPointF>& getVertices() const;
    void AddVertex(const QPointF& vertex);
    void UpdateLastVertex(const QPointF& new_vertex);
    void RemoveLastVertex();
    bool IsEmpty() const;
    std::optional<QPointF> IntersectRay(const Ray& ray) const;

private:
    std::vector<QPointF> vertices_;
};