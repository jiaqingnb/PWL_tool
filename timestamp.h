#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <QObject>

class Timestamp : public QObject
{
    Q_OBJECT
public:
    explicit Timestamp(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TIMESTAMP_H