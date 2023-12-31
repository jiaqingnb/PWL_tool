#include "mainwindow.h"
#include <QApplication>
#include "log.h"

#define VER_PROJECT  07
#define VER_MAJOR    00
#define VER_MINOR    02

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(bEnableDebugLog)
    {
        qInstallMessageHandler(outputMessage);
    }
    MainWindow w;
    w.show();
    w.setWindowTitle("PWL分析工具 V7.0.2");
    qDebug()<<"Version:"<< VER_PROJECT<<"." << VER_MAJOR<<"." << VER_MINOR;



    return a.exec();
}
