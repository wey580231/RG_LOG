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
**20160822:wey:��ӶԼ��ϡ�����ȼ�¼
**20160823:wey:��Ӷ�ջ�����С������ļ�¼
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
#define MAX_SIZE_PER_TEXT     10*1024*1024    //10��

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

//��ȡ���ͣ����ַ�����
template<typename T>
QString GetType(T t)
{
    return QString(typeid(t).name());
}

//�ж��Ƿ�ΪQString�����ַ���
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

//�ж��Ƿ�Ϊ�Զ������ͣ�������򷵻�false
#ifndef CHECK_LEGAL_TYPE
#define  CHECK_LEGAL_TYPE(T) \
    if(GetType(T).contains("class") && !GetType(T).toUpper().contains("QSTRING")) \
        return;
#endif

//��¼QList���Է�QString���͵���Ҫ����
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

//��¼QMap���Է�QString���͵���Ҫ����
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

//��¼QMap���Է�QString���͵���Ҫ����
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
