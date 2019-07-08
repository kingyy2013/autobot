#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    autobot::MainWindow w;
    w.show();
    w.move(height * 0.4, w.pos().y());

    return a.exec();
}
