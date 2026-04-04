#include "controller.h"
#include <algorithm>
#include <cmath>

namespace {
    double GetDistance(const QPointF& p1, const QPointF& p2) {
        return std::sqrt(std::pow(p1.x() - p2.x(), 2) + std::pow(p1.y() - p2.y(), 2));
    }
}

Controller::Controller() {
    std::vector<QPointF> bounds = {
        QPointF(-10, -10), QPointF(2000, -10),
        QPointF(2000, 2000), QPointF(-10, 2000)
    };
    polygons_.push_back(Polygon(bounds));
}

const std::vector<Polygon>& Controller::GetPolygons() const { return polygons_; }
void Controller::AddPolygon(const Polygon& poly) { polygons_.push_back(poly); }

void Controller::AddVertexToLastPolygon(const QPointF& new_vertex) {
    if (!polygons_.empty()) polygons_.back().AddVertex(new_vertex);
}

void Controller::UpdateLastPolygon(const QPointF& new_vertex) {
    if (!polygons_.empty()) polygons_.back().UpdateLastVertex(new_vertex);
}

void Controller::Undo() {
    if (polygons_.size() <= 1) return;
    auto& last_poly = polygons_.back();
    last_poly.RemoveLastVertex();
    if (last_poly.getVertices().size() < 1) {
        polygons_.pop_back();
    }
}

void Controller::RemoveLastPolygon() {
    if (polygons_.size() > 1) {
        polygons_.pop_back();
    }
}

QPointF Controller::getLightSource() const { return light_source_; }
void Controller::setLightSource(const QPointF& source) { light_source_ = source; }

std::vector<Ray> Controller::CastRays(const QPointF& origin) const {
    std::vector<Ray> rays;
    for (const auto& poly : polygons_) {
        for (const auto& vertex : poly.getVertices()) {
            double angle = std::atan2(vertex.y() - origin.y(), vertex.x() - origin.x());
            Ray base_ray(origin, vertex, angle);
            base_ray = base_ray.Rotate(0);
            rays.push_back(base_ray);
            rays.push_back(base_ray.Rotate(0.0001));
            rays.push_back(base_ray.Rotate(-0.0001));
        }
    }
    return rays;
}

void Controller::IntersectRays(std::vector<Ray>* rays) const {
    for (auto& ray : *rays) {
        double min_dist = GetDistance(ray.getBegin(), ray.getEnd());
        for (const auto& poly : polygons_) {
            auto intersection = poly.IntersectRay(ray);
            if (intersection.has_value()) {
                double dist = GetDistance(ray.getBegin(), intersection.value());
                if (dist < min_dist) {
                    min_dist = dist;
                    ray.setEnd(intersection.value());
                }
            }
        }
    }
}

Polygon Controller::CreateLightArea(const QPointF& origin) const {
    std::vector<Ray> rays = CastRays(origin);
    std::sort(rays.begin(), rays.end(), [](const Ray& a, const Ray& b) {
        return a.getAngle() < b.getAngle();
    });
    IntersectRays(&rays);
    std::vector<QPointF> light_vertices;
    for (const auto& ray : rays) {
        light_vertices.push_back(ray.getEnd());
    }
    return Polygon(light_vertices);
}

std::vector<Polygon> Controller::CreateSoftShadowAreas() const {
    std::vector<Polygon> light_areas;
    double offset = 3.0; 
    for (double dx = -offset; dx <= offset; dx += offset) {
        for (double dy = -offset; dy <= offset; dy += offset) {
            QPointF src(light_source_.x() + dx, light_source_.y() + dy);
            light_areas.push_back(CreateLightArea(src));
        }
    }
    return light_areas;
}