#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T09:06:11
#
#-------------------------------------------------

QT       -= gui

TARGET = RG_LOG
TEMPLATE = lib

DEFINES += RG_LOG_LIBRARY

SOURCES += logging.cpp \
    logthread.cpp

HEADERS += logging.h\
    logthread.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
