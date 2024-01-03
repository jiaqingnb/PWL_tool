#include "packet_loss.h"
#include "common.h"
#include "timestamp.h"

#define SOC  0x00000001
#define PREQ 0x00000003
#define PRES 0x00000004
#define SOA  0x00000005

#define PWLFRAMEOFFSET 42 //pwl协议帧类型偏移（pres,preq,soa,soc）
#define DESOFFSET      43 //目的节点偏移
#define SRCOFFSET      44 //源节点偏移

common com;//创建一个变量转换类变量
QFile lossfile;//需要做解析的文件
s_packloss* packloss = new s_packloss;//定义一个丢包率结构体指针

//QString STR;

/*
 * 描述：packet_loss.C函数解决计算各个节点丢包率问题
 * 需求：统计各帧的数量（soc、各preq、各pres）、
 *      时间总量（精确至0.001ms）统计。
 * 构想：使用脱包后的文件，逐包解析pres帧是
 *
 */
extern uint8_t g_PwlNodeId[PWL_NODE_NUM];
/*节点初始化*/
void PWlloss::NodeInit()
{
    uint8_t i;
    packloss->soacnt = 0;
    packloss->soccnt = 0;
    for(i=0;i<PWL_NODE_NUM;i++)
    {
       packloss->pwlnode[i].node = g_PwlNodeId[i];
       packloss->pwlnode[i].preqcnt = 0;
       packloss->pwlnode[i].prescnt = 0;
    }
}
/*设置解析文件路径*/
void PWlloss::setname(QString path)
{
    /*路径配置完成*/
    lossfile.setFileName(path);
    qDebug()<<path;
    /*直接解析丢包率*/
    Lossparsing();
}
/*丢包率解析函数*/
void PWlloss::Lossparsing()
{
    QDataStream srcStream(&lossfile);
    uint8_t  t_temp[1024 * 2];
    uint32_t t_len = 0u;
    uint32_t t_type = 0u;
    uint64_t fileSize = 0u;
    QFileInfo fileInfo(lossfile.fileName());
    fileSize = fileInfo.size();
    uint64_t i = 0u;

    lossfile.open(QFile::ReadOnly);

    qDebug()<<"Lossparsing";

    while(i < fileSize)
    {
        /* 数据类型 */
        srcStream.readRawData((char *)&t_temp[0], 4);
        t_type = com.LongFromCharLE(&t_temp[0]);

        /* 数据长度 */
        srcStream.readRawData((char *)&t_temp[4], 4);
        t_len = com.LongFromCharLE((uint8_t *)&t_temp[4]);

        //qDebug()<<"len = "<<t_len;
        /* 写入数据内容 */
        srcStream.readRawData((char *)&t_temp[8], t_len - 8);
        /* 如果是数据包 */
        if(t_type == DATA_TYPE)
        {
           /* 帧解析 */
          PWlloss::Frame_type(&t_temp[0]);


        }
        i = i + t_len;
        emit showBarValue(((i * 100) / fileSize));
    }

    lossfile.close();

}
/*根据不同的pwl协议帧类型做不同的处理*/
void PWlloss::Frame_type(uint8_t* data)
{
    uint8_t frame = *(data+PWLFRAMEOFFSET);
    uint8_t index= 0xff;
    switch (frame)
    {
    case SOC:
            packloss->soccnt++;
#if 1  //测试代码——测试时间戳解析函数
            if(1)
            {
              uint64_t timeLen = com.LongFromIntLE(&data[12]);
              QString tme = Timestamp::Gettime().TimestampToTime1(timeLen,4);
              qDebug()<<tme;
            }
#endif

        break;
    case PREQ:
            index = Searching_node(data[DESOFFSET]);
            if(index != 0xff)
            {
              packloss->pwlnode[index].preqcnt++;
            }
        break;
    case PRES:
            index = Searching_node(data[SRCOFFSET]);
            if(index != 0xff)
            {
              packloss->pwlnode[index].prescnt++;

            }
            board->parsedta(data);
        break;
    case SOA:
            packloss->soacnt++;
        break;
    }
}

uint8_t PWlloss::Searching_node(uint8_t type)
{
    uint8_t i =0;
    uint8_t index = 0xff;
    for (i=0;i<PWL_NODE_NUM;i++)
    {
        if(g_PwlNodeId[i] == type)
        {
            index = i;
        }
    }
    return index;
}



