#ifndef PACKET_LOSS_H
#define PACKET_LOSS_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDebug>
#include "QFileDialog"
#include <QDirIterator>
#include <QFileInfo>
//#include "RecvThread.h"
#include "common.h"
#include "board_mapping/board_mapping.h"

#define PWL_NODE_NUM 23
#define ROW_MAX     1000
#define DATA_TYPE 0x00000006

class PWlloss : public QObject
{
    Q_OBJECT

public:
    static PWlloss& loss()
    {
        static PWlloss loss;
        return loss;
    }
    void NodeInit();
    void setname(QString path);
    board_mapping* board = new board_mapping;
private:
    void Lossparsing();
    void Frame_type(uint8_t* data);
    uint8_t Searching_node(uint8_t type);/*查询节点表*/

signals:
    void showBarValue(int);
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
    uint64_t soctime;//SOC时间戳
    uint64_t soclasttime;//上一个SOC时间戳
    s_node  pwlnode[PWL_NODE_NUM];//节点
}s_packloss;

extern s_packloss* packloss;

extern QFile lossfile;

extern common com;



#endif // PACKET_LOSS_H
