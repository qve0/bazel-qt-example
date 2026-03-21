#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800, 500);
    w.show();
    return a.exec();
}