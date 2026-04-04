#include "polygon.h"
#include <cmath>

Polygon::Polygon(const std::vector<QPointF>& vertices) : vertices_(vertices) {}

const std::vector<QPointF>& Polygon::getVertices() const {
    return vertices_;
}

void Polygon::AddVertex(const QPointF& vertex) {
    vertices_.push_back(vertex);
}

void Polygon::UpdateLastVertex(const QPointF& new_vertex) {
    if (!vertices_.empty()) {
        vertices_.back() = new_vertex;
    }
}

void Polygon::RemoveLastVertex() {
    if (!vertices_.empty()) {
        vertices_.pop_back();
    }
}

bool Polygon::IsEmpty() const {
    return vertices_.empty();
}

std::optional<QPointF> Polygon::IntersectRay(const Ray& ray) const {
    if (vertices_.size() < 2) return std::nullopt;

    std::optional<QPointF> closest_intersection = std::nullopt;
    double min_t = 1e9;

    QPointF P = ray.getBegin();
    QPointF D = QPointF(ray.getEnd().x() - P.x(), ray.getEnd().y() - P.y());

    for (size_t i = 0; i < vertices_.size(); ++i) {
        QPointF A = vertices_[i];
        QPointF B = vertices_[(i + 1) % vertices_.size()];
        QPointF E = QPointF(B.x() - A.x(), B.y() - A.y());

        double det = -D.x() * E.y() + D.y() * E.x();
        if (std::abs(det) < 1e-7) continue;

        double t = ((A.x() - P.x()) * (-E.y()) - (A.y() - P.y()) * (-E.x())) / det;
        double u = (D.x() * (A.y() - P.y()) - D.y() * (A.x() - P.x())) / det;

        if (t > 0 && u >= 0 && u <= 1.0) {
            if (t < min_t) {
                min_t = t;
                closest_intersection = QPointF(P.x() + t * D.x(), P.y() + t * D.y());
            }
        }
    }
    return closest_intersection;
}