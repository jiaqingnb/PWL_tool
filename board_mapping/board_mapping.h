#ifndef BOARD_MAPPING_H
#define BOARD_MAPPING_H

#include <QString>
#include <QVector>
#include "common.h"

#define NODENEM 18

#define NODEDATA 44
#define PACGENUM 52

#define DATALEN 1024*1024

typedef struct
{
  uint32_t  node;
  uint16_t  end_data;
  uint16_t  data[DATALEN];
  uint16_t  start_data;
  uint16_t  reaend_data;
}s_boardmap;

class board_mapping
{
public:

    board_mapping();

    void getnodenum(uint8_t node,uint16_t packnum);

    void parsedta(uint8_t* data);

    s_boardmap boardsta[18];
private:


};

extern common com;

#endif // BOARD_MAPPING_H
