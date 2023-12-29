#include "syswatcher.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>


syswatcher::syswatcher()
{
     watcher = new QFileSystemWatcher;
}
/*添加监控文件的路径*/
void syswatcher::addWatchPath(QString path)
{
    bool ret;

    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryUpdated(QString)));
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));
    ret = watcher->addPath(path);
    if(ret == true)
        qDebug()<<"Monitoring success:"<<path;
    // 如果添加路径是一个目录，保存当前内容列表
    QFileInfo file(path);
    if (file.isDir())
    {
        qDebug()<<"save success:";
        const QDir dirw(path);
        p_currentContentsmap[path] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    }


}

void syswatcher::directoryUpdated(const QString &path)
{
    const QDir dirw(path);
    // 比较最新的内容和保存的内容找出区别(变化)
    QStringList currEntryList = p_currentContentsmap[path];
    const QDir dir(path);

    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
    QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

    // 添加了文件
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList arrnewFile = newFiles.toList();

    // 添加新文件/目录至Dir
    if (!arrnewFile.isEmpty())
    {
        p_currentContentsmap[path] = newEntryList;
        lastnewfile = newfile;
        newfile = path+"\\"+ arrnewFile[0];


        emit newfileOver();
        qDebug()<<"文件新增"<<newfile;
    }


}


void syswatcher::fileUpdated(const QString &path)
{
    qDebug()<<"文件改变1";
}
