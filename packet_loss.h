#ifndef PACKET_LOSS_H
#define PACKET_LOSS_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDebug>
#include "QFileDialog"
#include <QDirIterator>
#include <QFileInfo>
#include "RecvThread.h"
#include "common.h"
#include "board_mapping/board_mapping.h"

class PWlloss
{
public:
    static PWlloss& loss()
    {
        static PWlloss loss;
        return loss;
    }
    void NodeInit();
    void setname(QString path);
    void TimestampToTime1(uint64_t time, uint8_t g_frame);
    board_mapping* board = new board_mapping;
private:
    void Lossparsing();
    void Frame_type(uint8_t* data);
    uint8_t Searching_node(uint8_t type);/*查询节点表*/
};


typedef struct
{
    uint8_t node;//节点
    uint32_t prescnt;//PRES计数器
    uint32_t preqcnt;//PREQ计数器
}s_node;
typedef struct
{
    uint32_t soacnt;//SOA计数器
    uint32_t soccnt;//SOC计数器
    s_node  pwlnode[PWL_NODE_NUM];//节点
}s_packloss;

extern s_packloss* packloss;

extern QFile lossfile;

extern common com;



#endif // PACKET_LOSS_H
