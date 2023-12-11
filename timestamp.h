#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QObject>
#include <QDebug>

class Timestamp : public QObject
{
    Q_OBJECT
public:
    explicit Timestamp(QObject *parent = nullptr);

    static Timestamp& Gettime()
    {
        static Timestamp timestamp;
        return timestamp;
    }

    QString& TimestampToTime1(uint64_t time, uint8_t g_frame);


signals:

public slots:
};

#endif // TIMESTAMP_H
