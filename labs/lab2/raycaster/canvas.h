#pragma once
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include "controller.h"

enum class AppMode { Light, Polygons };

class CanvasWidget : public QWidget {
    Q_OBJECT
public:
    explicit CanvasWidget(QWidget* parent = nullptr);
    void setMode(AppMode mode);
    Controller& getController();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Controller controller_;
    AppMode current_mode_ = AppMode::Light;
    bool is_drawing_polygon_ = false;
};