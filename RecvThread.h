#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDebug>
#include "QFileDialog"
#include <QDirIterator>
#include <QFileInfo>

#define PWL_NODE_NUM 23
#define ROW_MAX     1000
#define DATA_TYPE 0x00000006

class RecvThread : public QThread
{
    Q_OBJECT

public:
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
    void WriteDataToBuf();
    void MakeNewFile();

signals:
    void analysisOver();
    void showBarValue(int);
};

#endif // RECVTHREAD_H
