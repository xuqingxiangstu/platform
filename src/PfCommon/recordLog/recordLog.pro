#-------------------------------------------------
#
# Project created by QtCreator 2020-01-07T11:14:47
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../recordLog
TEMPLATE = lib
CONFIG += C++11
DEFINES += RECORDLOG_LIBRARY

SOURCES += recordlog.cpp

HEADERS += recordlog.h\
        recordlog_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
