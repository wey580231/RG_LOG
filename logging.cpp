#include "logging.h"

#include <QMutex>
#include <QDateTime>
#include <QDebug>
#include <QTextCodec>
#include <QQueue>

#include "logthread.h"

QMutex ququeMutex;
QQueue<Logging::LogRecord> logQueue;     //日志集合

Logging::Logging()
{
    logging = this;

    logThread = NULL;
}

Logging * Logging::logging = NULL;

Logging * Logging::instance()
{
    return logging;
}

///*****************************************************
///**Function:log
///**Description:依照不同的记录级别记录信息
///**Input: LogLevel：日志记录级别
///         QString：日志信息
///**Output:
///**Return:
///**Others:
///****************************************************/
void Logging::log(const LogLevel level, const QString info)
{
    ququeMutex.lock();

    LogRecord record;
    record.timeStamp = getTimeStamp(true);
    record.level = level;
    record.content = info;

    logQueue.enqueue(record);

    ququeMutex.unlock();

    if(!logThread)
    {
        logThread = new LogThread;
    }

    logThread->startThread();


}

///*****************************************************
///**Function:log
///**Description:16进制记录数据
///**Input: LogLevel level：日志级别
///         const char *：待记录的数据
///         int ：数据长度
///**Output:
///**Return:
///**Others:
///****************************************************/
void Logging::log(const LogLevel level, const char *data, int dataLength)
{
    if(data == NULL)
    {
        return;
    }

    dataLength = dataLength > strlen(data) ? strlen(data) : dataLength;

    int buffLen = dataLength * 4;
    char * dest = new char[buffLen];
    memset(dest,0,sizeof(char)*buffLen);

    strToHex(data,dataLength,dest);

    log(level,QString(dest));

    delete [] dest;
}

//将数字转换成16进制表示
char Logging::valueToHex(int value)
{
    char result = '0';
    if(value>=0&&value<=9)
    {
        result = (char)(value+48);      //48为0的ascii码编码
    }
    else if(value>=10&&value<=15)
    {
        result = (char)(value-10+65);   //65为大写A的ascII码编码
    }
    return result;
}

//将字符串转换成16进制
void Logging::strToHex(const char * source,int sourceLen,char * dest)
{
    int index=0;
    int high,low,tmpValue;

    while(index<sourceLen)
    {
        tmpValue = *source;
        high = (tmpValue>>4)&0x0f;
        low = tmpValue&0x0f;

        *dest++ = valueToHex(high);
        *dest++ = valueToHex(low);
        *dest++ = ' ';            //可以根据实际需要加入间隔符

        source++;
        index++;
    }
}

void Logging::setCodec(QTextCodec *codec)
{
//    this->codec = codec;
}

///*****************************************************
///**Function:
///**Description:获取当前时间的时间戳
///**Input:   bool:是否需要详细的信息(精确到毫秒)
///**Output:
///**Return:QString:时间戳信息
///**Others:
///****************************************************/
QString Logging::getTimeStamp(bool isNeedDesc)
{
    QString format;
    if(isNeedDesc)
    {
        format = "yyyy-MM-dd hh:mm:ss:zzz";
    }
    else
    {
        format = "yyyyMMddhhmmss";
    }
    return QDateTime::currentDateTime().toString(format);
}

Logging::~Logging()
{
    if(logThread)
    {
        delete logThread;
        logThread = NULL;
    }
    if(logging ==  this)
    {
        logging = NULL;
    }
}
