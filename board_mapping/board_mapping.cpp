#include "board_mapping.h"

uint8_t NodeId[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,17, 18, 243, 244, 241, 242};

/*包序号分析构造函数，初始化管理器*/
board_mapping::board_mapping()
{

    for (uint8_t i = 0;i<18;i++) {
        boardsta[i].node = NodeId[i];
        boardsta[i].page = 0;
        memset(boardsta[i].mangerpro,0,sizeof(boardsta[i].mangerpro));
    }

}
void board_mapping::socdate(uint8_t* data)
{

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
    uint8_t page = 0;
    for (uint8_t j=0;j<18;j++)
    {
        page = boardsta[j].page;
        if((node == boardsta[j].node) && (boardsta[j].mangerpro[page].data[packnum] == 0))
        {
            /*该翻页了*/
            if((packnum == 0xffff) && (boardsta[j].page < PAGEPRONUM-1))
            {
                boardsta[j].page++;
            }

            boardsta[j].mangerpro[page].data[packnum] = 1;
            /*结束包序号赋值*/
            boardsta[j].mangerpro[page].reaend_data = packnum;
            /*起始包序号赋值*/
            if(boardsta[j].mangerpro[page].start_data ==0)
            {
              boardsta[j].mangerpro[page].start_data = packnum;
            }
            /*最大包序号赋值*/
            if(packnum>boardsta[j].mangerpro[page].end_data || boardsta[j].mangerpro[page].end_data ==0)
            {
              boardsta[j].mangerpro[page].end_data = packnum;
            }
        }
    }
}
/*清除管理器数据*/
void board_mapping::ClearManagerPro()
{
    for (uint8_t i = 0;i<18;i++) {
        boardsta[i].page = 0;
        memset(boardsta[i].mangerpro,0,sizeof(boardsta[i].mangerpro));
    }
}
