#include "logthread.h"

#include <QQueue>
#include <QDebug>

#include "logging.h"

extern QMutex QuqueMutex;
extern QQueue<Logging::LogRecord> logQueue;     //日志集合

LogThread::LogThread(QObject *parent):
    QThread(parent),codec(NULL)
{
    fileOpen = false;
    isRun = false;
    fileIndex = 1;

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
        QuqueMutex.lock();
        while(logQueue.size() > 0)
        {
            if(file.size() >= MAX_SIZE_PER_TEXT)
            {
                closeFile();
                fileIndex++;
                openFile();
            }

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
        QuqueMutex.unlock();

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
        //确保文件名只创建一次
        if(fileIndex == 1)
        {
            baseFileName = Logging::getTimeStamp();
        }

        QString fileName = baseFileName+"("+QString::number(fileIndex)+")"+".txt";
        qDebug()<<fileName;
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
        qDebug()<<"====closeFile=====";
//        LOG(Logging::NORMAL,"Finih!!!");
        file.close();
        fileOpen = false;
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
