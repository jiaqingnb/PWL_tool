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
              uint64_t timeLen = com.LongFromIntLE(&data[12]);
              QString tme = Timestamp::Gettime().TimestampToTime1(timeLen,4);
              //qDebug()<<tme;
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

///* 时间戳转化时间*/
//void PWlloss::TimestampToTime1(uint64_t time, uint8_t g_frame)
//{
//    //qDebug() << "进入时间戳";
//    uint32_t ms;                 //毫秒
//    uint64_t secondTime;        //秒
//    int minuteTime;         //分
//    int hourTime;           //时
//    int dayTime;            //天
//    int mouthTime;          //月
//    int yearTime;           //年
//    const uint8_t Days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//    const uint8_t DaysRun[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//    uint Pass4year;
//    uint hours_per_year;
//    QString str = "";
//    QString frame_number;
//    int i;

//    ms = time % 1000000;               //毫秒
//    secondTime = time / 1000000;       //获取秒总时长
//    time /= 1000000;

//    //取秒时间
//    secondTime = time % 60;
//    time /= 60;
//    //取分钟时间
//    minuteTime = time % 60;
//    time /= 60;
//    //取过去多少个四年，每四年有 1461*24小时
//    Pass4year = time / (1461L * 24L);
//    //计算年份
//    yearTime = (Pass4year << 2) + 1970;
//    //qDebug() << "计算年份: " << yearTime;
//    //四年中剩下的小时数
//    time = time % (1461L * 24L);
//    //校正闰年影响的年份，计算一年中剩下的小时数
//    for(;;)
//    {
//        //一年的小时数
//        hours_per_year = 365 * 24;
//        //判断闰年
//        if((yearTime % 4 == 0 && yearTime % 100 != 0) || yearTime % 400 == 0)
//        {
//            //是闰年，一年多24小时
//            hours_per_year += 24;
//        }
//        if(time < hours_per_year)
//        {
//            break;
//        }
//        yearTime++;                 //年
//        time -= hours_per_year;
//    }
//    //小时数
//    hourTime = time % 24;
//    hourTime += 8;
//    //一年中剩余的天数
//    time /= 24;
//    if((yearTime % 4 == 0 && yearTime % 100 != 0) || yearTime % 400 == 0)   //是闰年
//    {
//        for(i = 1; i <= 12; i++)
//        {
//            if(time > DaysRun[i])
//            {
//                time = time - DaysRun[i];
//            }
//            else if(time == DaysRun[i])
//            {
//                time = time - DaysRun[i];
//                i++;
//            }
//            else
//            {
//                break;
//            }
//        }
//        time++;
//    }
//    else        //不是闰年
//    {
//        for(i = 1; i <= 12; i++)
//        {
//            if(time > Days[i])
//            {
//                time = time - Days[i];
//            }
//            else if(time == Days[i])
//            {
//                time = time - Days[i];
//                i++;
//            }
//            else
//            {
//                break;
//            }
//        }
//        time++;

//    }
//    mouthTime = i;      //月
//    dayTime = time;     //日

//    /* 将时间戳补齐为yyyy-MM-dd HH:mm:ss.msmsms*/
//    QString yearTimer1 = QString("%1").arg(yearTime, 4, 10, QLatin1Char('0'));
//    QString mouthTimer1 = QString("%1").arg(mouthTime, 2, 10, QLatin1Char('0'));
//    QString dayTimer1 = QString("%1").arg(dayTime, 2, 10, QLatin1Char('0'));
//    QString hourTimer1 = QString("%1").arg(hourTime, 2, 10, QLatin1Char('0'));
//    QString minuteTimer1 = QString("%1").arg(minuteTime, 2, 10, QLatin1Char('0'));
//    QString secondTimer1 = QString("%1").arg(secondTime, 2, 10, QLatin1Char('0'));
//    QString msr1 = QString("%1").arg(ms, 6, 10, QLatin1Char('0'));

//    /* 输出时间戳对应时间*/
//    str = yearTimer1 + "-" + mouthTimer1 + "-" + dayTimer1 + QString(" ") +
//            hourTimer1 + ":" + minuteTimer1 + ":" + secondTimer1 + "." + msr1;

//    //对信息帧进行选择
//    if(g_frame == 1)
//    {
//       frame_number = "SoC";
//    }
//    else if(g_frame == 3)
//    {
//       frame_number = "PReq";
//    }
//    else if(g_frame == 4)
//    {
//       frame_number = "PRes";
//    }
//    else if(g_frame == 5)
//    {
//       frame_number = "SoA";
//    }

//    str = str + "         " + frame_number + "\n";
//    qDebug() << "时间戳 = "<<str;
//    /* 发送信号时间戳为红色*/
//    STR = str;
//}

