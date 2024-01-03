#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDebug>
#include "QFileDialog"
#include <QDirIterator>
#include <QFileInfo>
#include "packet_loss.h"



class RecvThread : public QThread
{
    Q_OBJECT

public:
    PWlloss* p_packloss = new PWlloss;
    explicit RecvThread(QObject *parent = nullptr);
    void SetFilePath(QString t_filepath);
    void SetSourceFilePath(QString t_filepath);
    uint32_t LongFromChar(const uint8_t *pInput);
    uint32_t LongFromCharLE(const uint8_t *pInput);
    QString GetFilePath();

protected:
    void run();

private:

    QFile srcFile;
    QFile destFile;
    QString srcFilePath;
    QString destFilePath;
    bool m_stop = false;
    void WriteDataToBuf(PWlloss* p_packloss);
    void MakeNewFile();
    PWlloss* p_func(PWlloss* p_packloss);

signals:
    void analysisOver();
    void showBarValue(int);
};

#endif // RECVTHREAD_H
