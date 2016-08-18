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
**
*************************************************/
#ifndef LOGGING_H
#define LOGGING_H

#include <QString>
#include <QTextStream>
#include <QFile>

class QTextCodec;

#include "rg_log_global.h"

#define LOG (Logging::instance()->log)

class RG_LOGSHARED_EXPORT Logging
{
public:
    enum LogLevel
    {
        NORMAL,
        WARN,
        ERROR
    };

    Logging();
    ~Logging();

    static Logging * instance();
    void setCodec(QTextCodec * codec);

    void log(const LogLevel level,const QString info);
    void log(const LogLevel level,const char * data,int dataLength);

private:
    void openFile();
    void closeFile();
    QString getTimeStamp(bool isNeedDesc = false);
    QString getWrappedText(QString text);

    char valueToHex(int value);
    void strToHex(const char * source, int sourceLen, char * dest);

    static Logging * logging;

    bool fileOpen;

    QFile file;
    QTextStream stream;

    QTextCodec * codec;

};

#endif // LOGGING_H
