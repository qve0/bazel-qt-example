#include "canvas.h"
#include <QPainter>
#include <QPolygonF>
#include <QKeyEvent>

CanvasWidget::CanvasWidget(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void CanvasWidget::setMode(AppMode mode) {
    if (current_mode_ == AppMode::Polygons && is_drawing_polygon_) {
        controller_.Undo(); 
        is_drawing_polygon_ = false;
    }
    current_mode_ = mode;
    setFocus();
    update();
}

Controller& CanvasWidget::getController() { return controller_; }

void CanvasWidget::mousePressEvent(QMouseEvent* event) {
    QPointF pos = event->position();
    if (current_mode_ == AppMode::Polygons) {
        if (event->button() == Qt::LeftButton) {
            if (!is_drawing_polygon_) {
                Polygon p;
                p.AddVertex(pos);
                p.AddVertex(pos);
                controller_.AddPolygon(p);
                is_drawing_polygon_ = true;
            } else {
                controller_.UpdateLastPolygon(pos);
                controller_.AddVertexToLastPolygon(pos);
            }
        } else if (event->button() == Qt::RightButton && is_drawing_polygon_) {
            controller_.Undo();
            is_drawing_polygon_ = false;
        }
        update();
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event) {
    QPointF pos = event->position();
    if (current_mode_ == AppMode::Light) {
        controller_.setLightSource(pos);
        update();
    } else if (current_mode_ == AppMode::Polygons && is_drawing_polygon_) {
        controller_.UpdateLastPolygon(pos);
        update();
    }
}

void CanvasWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ControlModifier)) {
        if (is_drawing_polygon_) {
            controller_.Undo();
            if (controller_.GetPolygons().back().getVertices().size() < 2) {
                is_drawing_polygon_ = false;
            }
        } else {
            controller_.RemoveLastPolygon();
        }
        update();
    }
}

void CanvasWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);

    painter.setPen(Qt::NoPen);
    QColor light_color(255, 255, 200, 25);
    painter.setBrush(light_color);
    
    auto light_areas = controller_.CreateSoftShadowAreas();
    for (const auto& poly : light_areas) {
        QPolygonF qpoly;
        for (const auto& v : poly.getVertices()) qpoly << v;
        painter.drawPolygon(qpoly);
    }

    painter.setBrush(Qt::white);
    painter.drawEllipse(controller_.getLightSource(), 4, 4);

    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(QPen(Qt::white, 1));
    const auto& polygons = controller_.GetPolygons();
    for (size_t i = 1; i < polygons.size(); ++i) {
        QPolygonF qpoly;
        for (const auto& v : polygons[i].getVertices()) qpoly << v;
        painter.drawPolygon(qpoly);
    }
}