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
**20160822:wey:添加对集合、链表等记录
**20160823:wey:添加对栈、队列、容器的记录
*************************************************/
#ifndef LOGGING_H
#define LOGGING_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QVector>
#include <QMap>

#include "typeinfo.h"

class LogThread;

#define LOG (Logging::instance()->log)
#define MAX_SIZE_PER_TEXT     10*1024*1024    //10兆

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
    void log(const LogLevel level,const QStringList listInfo,QString splitFlag = ",");

    template<typename T>
    void log(const LogLevel level,const QList<T> listInfo,QString splitFlag = ",");

    template<typename T,typename V>
    void log(const LogLevel level,const QMap<T,V> mapInfo, QString splitFlag = ",");

    template<typename T>
    void log(const LogLevel level,const QVector<T> vectorInfo,QString splitFlag = ",");

    void log(const LogLevel level,const char * data,int dataLength);

private:
    char valueToHex(int value);
    void strToHex(const char * source, int sourceLen, char * dest);

    static Logging * logging;

    LogThread * logThread;

};

#define ToString(s) #s

//获取类型，以字符返回
template<typename T>
QString GetType(T t)
{
    return QString(typeid(t).name());
}

//判断是否为QString类型字符串
template<typename T>
bool CheckStringType(T t)
{
    QString type = GetType(t);
    if(type.contains("class"))
    {
        return type.toUpper().contains("QSTRING");
    }
    return false;
}

//判断是否为自定义类型，如果是则返回false
#ifndef CHECK_LEGAL_TYPE
#define  CHECK_LEGAL_TYPE(T) \
    if(GetType(T).contains("class") && !GetType(T).toUpper().contains("QSTRING")) \
        return;
#endif

//记录QList，对非QString类型的类要屏蔽
template<typename T>
void Logging::log(const LogLevel level, const QList<T> listInfo, QString splitFlag)
{
    T t;
    CHECK_LEGAL_TYPE(t)

    bool isStringType = false;

    if(CheckStringType(t))
    {
        isStringType = true;
    }

    QString info;

    for(int i = 0; i < listInfo.size(); i++)
    {

        if(isStringType)
        {
           info += listInfo.at(i);
        }
        else
        {
            info += QString::number(listInfo.at(i));
        }

        if(i != listInfo.size()-1)
        {
            info += splitFlag;
        }
    }

    log(level,info);
}

//记录QMap，对非QString类型的类要屏蔽
template<typename T,typename V>
void Logging::log(const LogLevel level,const QMap<T,V> mapInfo, QString splitFlag)
{
    T t;
    V v;
    CHECK_LEGAL_TYPE(t)
    CHECK_LEGAL_TYPE(v)

    bool isKeyStringType = false;
    bool isValStringType = false;

    if(CheckStringType(t))
    {
        isKeyStringType = true;
    }

    if(CheckStringType(v))
    {
        isValStringType = true;
    }

    QString info;

    QList<T> keys = mapInfo.keys();

    for(int i = 0; i<keys.size(); i++)
    {
        if(isKeyStringType)
        {
            info += keys.at(i);
        }
        else
        {
            info += QString(ToString(keys.at(i)));
        }

        info += ":";

        if(isValStringType)
        {
            info += mapInfo.value(keys.at(i));
        }
        else
        {
            info += QString::number(mapInfo.value(keys.at(i)));
        }

        info += "]";

        if(i != keys.size()-1 )
        {
            info += splitFlag;
        }
    }

    log(level,info);
}

//记录QMap，对非QString类型的类要屏蔽
template <typename T>
void Logging::log(const LogLevel level,const QVector<T> vectorInfo,QString splitFlag)
{
    T t;
    CHECK_LEGAL_TYPE(t)

    bool isStringType = false;

    if(CheckStringType(t))
    {
        isStringType = true;
    }

    QString info;

    for(int i = 0; i < vectorInfo.size(); i++)
    {

        if(isStringType)
        {
           info += vectorInfo.at(i);
        }
        else
        {
            info += QString::number(vectorInfo.at(i));
        }

        if(i != vectorInfo.size()-1)
        {
            info += splitFlag;
        }
    }

    log(level,info);
}

#endif // LOGGING_H
