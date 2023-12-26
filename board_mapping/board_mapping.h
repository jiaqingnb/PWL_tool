#ifndef BOARD_MAPPING_H
#define BOARD_MAPPING_H

#include <QString>
#include <QVector>
#include "common.h"
#include <string.h>

#define NODENEM 18

#define NODEDATA 44     //节点号偏移地址
#define PACGENUM 52     //包序号偏移地址

#define DATALEN 65535
#define FirstNum 10     //起始包序号
#define LsatNum 5       //结束包序号

#define PAGEPRONUM   30    //页数量
#define MANAGERPRO  18    //管理器数量

typedef struct
{  
  uint16_t  end_data;
  uint16_t  start_data;
  uint16_t  reaend_data;
  uint16_t  data[DATALEN];
}s_ManagerPro;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
  uint32_t  node;//节点
  uint8_t page;//页号
  s_ManagerPro mangerpro[PAGEPRONUM];//页
}s_boardpage;
#pragma pack(pop)

class board_mapping
{
public:

    board_mapping();

    void socdate(uint8_t* data);

    void parsedta(uint8_t* data);

    void ClearManagerPro();

    s_boardpage boardsta[MANAGERPRO];//管理器
private:

    void getnodenum(uint8_t node,uint16_t packnum);

};

extern common com;

#endif // BOARD_MAPPING_H
