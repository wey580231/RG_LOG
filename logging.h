/*************************************************
**版  权：RenGu Company
**文件名: logging.h
**作  者: wey       Version: 1.0       Date: 2016.07.28
**描  述: 日志记录模块
**Others: 1：提供日志记录功能
**        2：提供不同等级日志记录功能，方便查阅
**        3：支持时间戳命名
**
**修改历史:
**20160808:wey:添加记录16进制数据
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
