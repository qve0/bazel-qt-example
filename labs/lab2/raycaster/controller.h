#pragma once
#include <vector>
#include <QPointF>
#include "polygon.h"
#include "ray.h"

class Controller {
public:
    Controller();

    const std::vector<Polygon>& GetPolygons() const;
    void AddPolygon(const Polygon& poly);
    void AddVertexToLastPolygon(const QPointF& new_vertex);
    void UpdateLastPolygon(const QPointF& new_vertex);
    void Undo();
    void RemoveLastPolygon();

    QPointF getLightSource() const;
    void setLightSource(const QPointF& source);

    std::vector<Ray> CastRays(const QPointF& origin) const;
    void IntersectRays(std::vector<Ray>* rays) const;
    Polygon CreateLightArea(const QPointF& origin) const;
    std::vector<Polygon> CreateSoftShadowAreas() const;

private:
    std::vector<Polygon> polygons_;
    QPointF light_source_;
};