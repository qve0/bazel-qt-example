#include "main_window.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Lab 2: 2D Raycaster");
    resize(800, 600);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* layout = new QVBoxLayout(central);

    mode_selector_ = new QComboBox(this);
    mode_selector_->addItem("Режим <light>");
    mode_selector_->addItem("Режим <polygons>");
    
    canvas_ = new CanvasWidget(this);
    canvas_->setFocusPolicy(Qt::StrongFocus);
    canvas_->setFocus();

    layout->addWidget(mode_selector_);
    layout->addWidget(canvas_, 1);

    connect(mode_selector_, &QComboBox::currentIndexChanged, this, [this](int index) {
        canvas_->setMode(index == 0 ? AppMode::Light : AppMode::Polygons);
    });
}