#pragma once
#include <QPointF>
#include <cmath>

class Ray {
public:
    Ray(const QPointF& begin, const QPointF& end, double angle);

    QPointF getBegin() const;
    void setBegin(const QPointF& begin);

    QPointF getEnd() const;
    void setEnd(const QPointF& end);

    double getAngle() const;
    void setAngle(double angle);

    Ray Rotate(double angle_offset) const;

private:
    QPointF begin_;
    QPointF end_;
    double angle_;
};