#include "RecvThread.h"
#include "common.h"

extern uint8_t g_PwlNodeId[PWL_NODE_NUM];

RecvThread::RecvThread(QObject *parent) : QThread(parent)
{

}

QString RecvThread::GetFilePath()
{
    return destFilePath;
}

void RecvThread::SetFilePath(QString t_filepath)
{
    QFileInfo fileInfo(t_filepath);
    srcFilePath = t_filepath;
    destFilePath = fileInfo.absolutePath() + "/PwlDate.dat";
}

void RecvThread::MakeNewFile()
{
    srcFile.setFileName(srcFilePath);
    destFile.setFileName(destFilePath);
    if(destFile.open(QFile::ReadWrite | QFile::Truncate))
    {
        qDebug()<< destFilePath << "open success";
        destFile.close();
    }
    else
    {
        qDebug()<< destFilePath << "open error";
    }
}

void RecvThread::run()
{
    qDebug()<<"RecvThread is start";
    WriteDataToBuf();
    emit analysisOver();
    quit();
    qDebug()<<"RecvThread is exit";
}

void RecvThread::WriteDataToBuf()
{
    QDataStream srcStream(&srcFile);
    QDataStream destStream(&destFile);
    char  t_temp[1024 * 2];
    uint32_t t_len = 0u;
    uint32_t t_type = 0u;

    uint64_t fileSize = 0u;
    QFileInfo fileInfo(srcFilePath);
    fileSize = fileInfo.size();
    uint64_t i = 0u;
    MakeNewFile();

    srcFile.open(QFile::ReadOnly);
    destFile.open(QFile::ReadWrite | QFile::Truncate);
    qDebug()<<"WriteDataToBuf";

    while(i < fileSize)
    {
        /* 数据类型 */
        srcStream.readRawData(&t_temp[0], 4);
        t_type = LongFromCharLE((uint8_t *)&t_temp[0]);

        /* 数据长度 */
        srcStream.readRawData(&t_temp[4], 4);
        t_len = LongFromCharLE((uint8_t *)&t_temp[4]);

        srcStream.readRawData(&t_temp[8], t_len - 8);

        if(t_type == DATA_TYPE)
        {
            destStream.writeRawData(&t_temp[0], t_len);
        }
        i = i + t_len;
        emit showBarValue(((i * 100) / fileSize));
    }

    srcFile.close();
    destFile.close();
}

uint32_t RecvThread::LongFromChar(const uint8_t *pInput)
{
    uint32_t Templong;
    Templong = ( *(pInput) );
    Templong = ( Templong<<8 ) + ( *(pInput+1) );
    Templong = ( Templong<<8 ) + ( *(pInput+2) );
    Templong = ( Templong<<8 ) + ( *(pInput+3) );
    return Templong;
}

uint32_t RecvThread::LongFromCharLE(const uint8_t *pInput)
{
    uint32_t Templong;
    Templong = ( *(pInput+3) );
    Templong = ( Templong<<8 ) + ( *(pInput+2) );
    Templong = ( Templong<<8 ) + ( *(pInput+1) );
    Templong = ( Templong<<8 ) + ( *(pInput) );
    return Templong;
}
