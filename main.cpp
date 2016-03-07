#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(0, "MainWindow");
    w.show();

    return a.exec();
}
