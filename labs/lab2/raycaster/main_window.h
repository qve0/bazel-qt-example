#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <QVBoxLayout>
#include "canvas.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
private:
    CanvasWidget* canvas_;
    QComboBox* mode_selector_;
};