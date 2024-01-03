#include "log.h"
#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>
#include <QMutex>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include <QFile>
#include <QtDebug>
#include <QMessageBox>

log::log(QObject *parent) : QObject(parent)
{

}



/**
* @projectName JplocalSense
* @brief 摘要 :日志输出
*    调用格式如下：
*     qDebug("This is a debug message");
*     qWarning("This is a warning message");
*     qCritical("This is a critical message");
*     qFatal("This is a fatal message");
*
* @date 2020-10-30
*/

const bool bEnableDebugLog =  0 ; //是否启用日志输出

void WriteLine(QString logFileName,  QString logMessage)
{
    QFile file(logFileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << logMessage << "\r\n";
    file.flush();
    file.close();
}


 void Log(QString logFileName,  QString logMessage)
{
    static QMutex mutex;
    mutex.lock();

    if (bEnableDebugLog)
    {
         WriteLine(logFileName, logMessage);
    }

    mutex.unlock();
}


void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);

    QString message;
    QString text;
    QString fileName;

    switch(type)
    {
    case QtDebugMsg:
       text = QString("Debug:");
       message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);
       fileName = "logDebug.txt";
       Log(fileName,message);
       break;

    case QtWarningMsg:
       text = QString("Warning:");
       message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);
       fileName = "logWarning.txt";
       Log(fileName,message);
       break;

    case QtCriticalMsg:
       text = QString("Critical:");
       message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);
       fileName = "logCritical.txt";
       Log(fileName,message);
       break;

    case QtFatalMsg:
        text = QString("Fatal:");
        message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);
        fileName = "logFatal.txt";
        Log(fileName,message);
        break;
    default:break;
    }
}
