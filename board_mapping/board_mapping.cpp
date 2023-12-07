#include "board_mapping.h"

uint8_t NodeId[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,17, 18, 243, 244, 241, 242};

/*包序号分析构造函数，初始化表属性*/
board_mapping::board_mapping()
{

    for ( uint8_t i = 0;i<18;i++) {
        boardsta[i].node = NodeId[i];
        boardsta[i].end_data = 0;
        boardsta[i].start_data = 0;
        boardsta[i].reaend_data = 0;
        for (uint32_t j =0;j<DATALEN;j++) {
            boardsta[i].data[j] = 0;
        }
    }

}
/*获取每包PRES数据*/
void board_mapping::parsedta(uint8_t* data)
{
    uint8_t node = 0xff;
    uint16_t t_packnum = 0;

    node = data[NODEDATA];
    t_packnum = com.ShortFromCharLE(&data[PACGENUM]);

    getnodenum(node,t_packnum);
}


void board_mapping::getnodenum(uint8_t node,uint16_t packnum)
{

    for (uint8_t i=0;i<18;i++) {
        if((node == boardsta[i].node) && (boardsta[i].data[packnum] == 0))
        {

            boardsta[i].data[packnum] = 1;
            boardsta[i].reaend_data = packnum;
            if(boardsta[i].start_data ==0)
            {
              boardsta[i].start_data = packnum;
            }
            if(packnum>boardsta[i].end_data || boardsta[i].end_data ==0)
            {
              boardsta[i].end_data = packnum;
            }
        }
    }
}
