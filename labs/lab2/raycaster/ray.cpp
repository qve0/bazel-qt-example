#include "ray.h"

Ray::Ray(const QPointF& begin, const QPointF& end, double angle)
    : begin_(begin), end_(end), angle_(angle) {}

QPointF Ray::getBegin() const { return begin_; }
void Ray::setBegin(const QPointF& begin) { begin_ = begin; }

QPointF Ray::getEnd() const { return end_; }
void Ray::setEnd(const QPointF& end) { end_ = end; }

double Ray::getAngle() const { return angle_; }
void Ray::setAngle(double angle) { angle_ = angle; }

Ray Ray::Rotate(double angle_offset) const {
    double new_angle = angle_ + angle_offset;
    double dx = std::cos(new_angle) * 10000.0;
    double dy = std::sin(new_angle) * 10000.0;
    return Ray(begin_, QPointF(begin_.x() + dx, begin_.y() + dy), new_angle);
}