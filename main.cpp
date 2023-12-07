#include "mainwindow.h"
#include <QApplication>

#define VER_PROJECT  07
#define VER_MAJOR    01
#define VER_MINOR    00

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("PWL分析工具 V7.0.0");
    qDebug()<<"Version:"<< VER_PROJECT<<"." << VER_MAJOR<<"." << VER_MINOR;

    return a.exec();
}
