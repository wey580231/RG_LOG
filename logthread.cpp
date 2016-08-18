#include "logthread.h"

#include <QQueue>
#include <QDebug>

#include "logging.h"

extern QMutex ququeMutex;
extern QQueue<Logging::LogRecord> logQueue;     //日志集合

LogThread::LogThread(QObject *parent):
    QThread(parent),codec(NULL)
{
    fileOpen = false;
    isRun = false;

    openFile();
}

void LogThread::startThread()
{
    if(!isRunning())
    {
        start();
    }
    else if(!isRun)
    {
        condition.wakeAll();
    }
}

void LogThread::run()
{
    while(true)
    {
        ququeMutex.lock();
        while(logQueue.size() > 0)
        {
            isRun = true;
            Logging::LogRecord  log = logQueue.dequeue();

            QString logLevel;
            switch(log.level)
            {
                case Logging::NORMAL:
                                     logLevel = getWrappedText("Info");
                                     break;
                case Logging::WARN:
                                     logLevel = getWrappedText("Warn");
                                     break;
                case Logging::ERROR:
                                     logLevel = getWrappedText("Error");
                                     break;
            }

            stream<<getWrappedText(log.timeStamp)
                    <<logLevel
                     <<":"
                      <<log.content
                       <<"\n";

            stream.flush();
        }
        ququeMutex.unlock();

        isRun = false;

        mutex.lock();
        condition.wait(&mutex);
        mutex.unlock();
    }
}

void LogThread::openFile()
{
    if(!fileOpen)
    {
        QString fileName = Logging::getTimeStamp()+".txt";
        file.setFileName(fileName);
        if(!file.open(QFile::WriteOnly|QIODevice::Text))
        {
            fileOpen = false;
        }
        else
        {
            stream.setDevice(&file);
//            stream.setCodec(codec);
            fileOpen = true;
//            LOG(Logging::NORMAL,"Start Logging!");
        }
    }
}

void LogThread::closeFile()
{
    if(fileOpen)
    {
        file.close();

        LOG(Logging::NORMAL,"Finih!!!");
    }
}

//将普通字符串利用中括号包裹
QString LogThread::getWrappedText(QString text)
{
    return QString("["+text+"]");
}

LogThread::~LogThread()
{
    closeFile();

    mutex.lock();
    condition.wait(&mutex);
    mutex.unlock();

    wait();
}
