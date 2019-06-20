#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    autobot::MainWindow w;
    w.show();

    return a.exec();
}
