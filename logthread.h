/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: logthread.h
**��  ��: wey       Version: 1.0       Date: 2016.08.18
**��  ��: ��־��¼�߳�
**Others: 1������־������ȡ�����ݲ���¼
**
**�޸���ʷ:
**20160818:wey:�����־��¼����
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

    bool fileOpen;                      //�ļ��Ƿ��
    bool isRun;                         //�߳��Ƿ�������

    QFile file;
    QTextStream stream;

    QTextCodec * codec;

    QWaitCondition condition;
    QMutex mutex;

};

#endif // LOGTHREAD_H
