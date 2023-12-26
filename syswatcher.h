#ifndef SYSWATCHER_H
#define SYSWATCHER_H

#include <qfilesystemwatcher.h>
#include <QMap>

class syswatcher : public QObject
{
    Q_OBJECT

private:
    QFileSystemWatcher* watcher;
    QMap<QString,QStringList> p_currentContentsmap;
public:
    syswatcher();
    void addWatchPath(QString path);
public slots:
    void directoryUpdated(const QString &path);  // 目录更新时调用，path是监控的路径
    void fileUpdated(const QString &path);   // 文件被修改时调用，path是监控的路径

};

#endif // SYSWATCHER_H
