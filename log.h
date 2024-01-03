#ifndef LOG_H
#define LOG_H

#include <QObject>

class log : public QObject
{
    Q_OBJECT
public:
    explicit log(QObject *parent = nullptr);

signals:

public slots:
};
extern const bool bEnableDebugLog;
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOG_H
