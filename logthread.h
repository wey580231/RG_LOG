/*************************************************
**版  权：RenGu Company
**文件名: logthread.h
**作  者: wey       Version: 1.0       Date: 2016.08.18
**描  述: 日志记录线程
**Others: 1：从日志集合中取出数据并记录
**
**修改历史:
**20160818:wey:添加日志记录功能
*************************************************/
#ifndef LOGTHREAD_H
#define LOGTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

#include <QTextStream>
#include <QFile>

class QTextCodec;

class LogThread : public QThread
{
    Q_OBJECT

public:
    LogThread( QObject * parent = 0);
    void startThread();
    bool isProcess(){return this->isRun;}
    ~LogThread();

protected:
    void run();

private:
    void openFile();
    void closeFile();
    QString getWrappedText(QString text);

    bool fileOpen;                      //文件是否打开
    bool isRun;                         //线程是否在运行

    QFile file;
    QTextStream stream;

    QTextCodec * codec;

    QWaitCondition condition;
    QMutex mutex;

};

#endif // LOGTHREAD_H
