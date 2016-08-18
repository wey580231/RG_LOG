#include "logging.h"

#include <QMutex>
#include <QDateTime>
#include <QDebug>
#include <QTextCodec>
#include <QQueue>

#include "logthread.h"

QMutex ququeMutex;
QQueue<Logging::LogRecord> logQueue;     //��־����

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
///**Description:���ղ�ͬ�ļ�¼�����¼��Ϣ
///**Input: LogLevel����־��¼����
///         QString����־��Ϣ
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
///**Description:16���Ƽ�¼����
///**Input: LogLevel level����־����
///         const char *������¼������
///         int �����ݳ���
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

//������ת����16���Ʊ�ʾ
char Logging::valueToHex(int value)
{
    char result = '0';
    if(value>=0&&value<=9)
    {
        result = (char)(value+48);      //48Ϊ0��ascii�����
    }
    else if(value>=10&&value<=15)
    {
        result = (char)(value-10+65);   //65Ϊ��дA��ascII�����
    }
    return result;
}

//���ַ���ת����16����
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
        *dest++ = ' ';            //���Ը���ʵ����Ҫ��������

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
///**Description:��ȡ��ǰʱ���ʱ���
///**Input:   bool:�Ƿ���Ҫ��ϸ����Ϣ(��ȷ������)
///**Output:
///**Return:QString:ʱ�����Ϣ
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
