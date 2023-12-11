#include "timestamp.h"

Timestamp::Timestamp(QObject *parent) : QObject(parent)
{

}

QString& Timestamp::TimestampToTime1(uint64_t time, uint8_t g_frame)
{
    //qDebug() << "进入时间戳";
    uint32_t ms;                 //毫秒
    uint64_t secondTime;        //秒
    int minuteTime;         //分
    int hourTime;           //时
    int dayTime;            //天
    int mouthTime;          //月
    int yearTime;           //年
    const uint8_t Days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const uint8_t DaysRun[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint Pass4year;
    uint hours_per_year;
    static QString str = "";
    QString frame_number;
    int i;

    ms = time % 1000000;               //毫秒
    secondTime = time / 1000000;       //获取秒总时长
    time /= 1000000;

    //取秒时间
    secondTime = time % 60;
    time /= 60;
    //取分钟时间
    minuteTime = time % 60;
    time /= 60;
    //取过去多少个四年，每四年有 1461*24小时
    Pass4year =static_cast<uint>(time / (1461L * 24L));
    //计算年份
    yearTime = static_cast<int>((Pass4year << 2) + 1970);
    //qDebug() << "计算年份: " << yearTime;
    //四年中剩下的小时数
    time = time % (1461L * 24L);
    //校正闰年影响的年份，计算一年中剩下的小时数
    for(;;)
    {
        //一年的小时数
        hours_per_year = 365 * 24;
        //判断闰年
        if((yearTime % 4 == 0 && yearTime % 100 != 0) || yearTime % 400 == 0)
        {
            //是闰年，一年多24小时
            hours_per_year += 24;
        }
        if(time < hours_per_year)
        {
            break;
        }
        yearTime++;                 //年
        time -= hours_per_year;
    }
    //小时数
    hourTime = time % 24;
    hourTime += 8;
    //一年中剩余的天数
    time /= 24;
    if((yearTime % 4 == 0 && yearTime % 100 != 0) || yearTime % 400 == 0)   //是闰年
    {
        for(i = 1; i <= 12; i++)
        {
            if(time > DaysRun[i])
            {
                time = time - DaysRun[i];
            }
            else if(time == DaysRun[i])
            {
                time = time - DaysRun[i];
                i++;
            }
            else
            {
                break;
            }
        }
        time++;
    }
    else        //不是闰年
    {
        for(i = 1; i <= 12; i++)
        {
            if(time > Days[i])
            {
                time = time - Days[i];
            }
            else if(time == Days[i])
            {
                time = time - Days[i];
                i++;
            }
            else
            {
                break;
            }
        }
        time++;

    }
    mouthTime = i;      //月
    dayTime = static_cast<int>(time);     //日

    /* 将时间戳补齐为yyyy-MM-dd HH:mm:ss.msmsms*/
    QString yearTimer1 = QString("%1").arg(yearTime, 4, 10, QLatin1Char('0'));
    QString mouthTimer1 = QString("%1").arg(mouthTime, 2, 10, QLatin1Char('0'));
    QString dayTimer1 = QString("%1").arg(dayTime, 2, 10, QLatin1Char('0'));
    QString hourTimer1 = QString("%1").arg(hourTime, 2, 10, QLatin1Char('0'));
    QString minuteTimer1 = QString("%1").arg(minuteTime, 2, 10, QLatin1Char('0'));
    QString secondTimer1 = QString("%1").arg(secondTime, 2, 10, QLatin1Char('0'));
    QString msr1 = QString("%1").arg(ms, 6, 10, QLatin1Char('0'));

    /* 输出时间戳对应时间*/
    str = yearTimer1 + "-" + mouthTimer1 + "-" + dayTimer1 + QString(" ") +
            hourTimer1 + ":" + minuteTimer1 + ":" + secondTimer1 + "." + msr1;

    //对信息帧进行选择
    if(g_frame == 1)
    {
       frame_number = "SoC";
    }
    else if(g_frame == 3)
    {
       frame_number = "PReq";
    }
    else if(g_frame == 4)
    {
       frame_number = "PRes";
    }
    else if(g_frame == 5)
    {
       frame_number = "SoA";
    }

    str = str + "         " + frame_number + "\n";
    //qDebug() << "时间戳 = "<<str;
    /* 发送信号时间戳为红色*/
    return str;
}
