/*************************************************
**��  Ȩ��RenGu Company
**�ļ���: logging.h
**��  ��: wey       Version: 1.0       Date: 2016.07.28
**��  ��: ��־��¼ģ��
**Others: 1���ṩ��־��¼����
**        2���ṩ��ͬ�ȼ���־��¼���ܣ��������
**        3��֧��ʱ�������
**
**�޸���ʷ:
**20160808:wey:��Ӽ�¼16��������
**20160818:wey:����̼߳�¼����
*************************************************/
#ifndef LOGGING_H
#define LOGGING_H

#include <QString>

class LogThread;

#define LOG (Logging::instance()->log)
#define MAX_SIZE_PER_TEXT     10*1000*1000    //10��

class Q_DECL_EXPORT Logging
{
public:
    enum LogLevel
    {
        NORMAL,
        WARN,
        ERROR
    };

    //һ����־��¼
    struct LogRecord
    {
        QString timeStamp;          //ʱ���
        LogLevel level;             //��־�ȼ�
        QString content;            //��־����
    };

    Logging();
    ~Logging();

    static Logging * instance();
    static QString getTimeStamp(bool isNeedDesc = false);
    void setCodec(QTextCodec * codec);

    void log(const LogLevel level,const QString info);
    void log(const LogLevel level,const char * data,int dataLength);

private:
    char valueToHex(int value);
    void strToHex(const char * source, int sourceLen, char * dest);

    static Logging * logging;

    LogThread * logThread;

};

#endif // LOGGING_H
