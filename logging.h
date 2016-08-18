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
**20160818:wey:添加线程记录数据
*************************************************/
#ifndef LOGGING_H
#define LOGGING_H

#include <QString>

class LogThread;

#define LOG (Logging::instance()->log)
#define MAX_SIZE_PER_TEXT     10*1000*1000    //10兆

class Q_DECL_EXPORT Logging
{
public:
    enum LogLevel
    {
        NORMAL,
        WARN,
        ERROR
    };

    //一条日志记录
    struct LogRecord
    {
        QString timeStamp;          //时间戳
        LogLevel level;             //日志等级
        QString content;            //日志内容
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
