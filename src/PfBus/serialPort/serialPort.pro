#-------------------------------------------------
#
# Project created by QtCreator 2019-12-27T15:44:00
#
#-------------------------------------------------

QT       -= gui
CONFIG += c++11
#TARGET = serialPort
TARGET = ../../../../serialPort
TEMPLATE = lib

DEFINES += SERIALPORT_LIBRARY

SOURCES += \
    serialport.cpp

HEADERS +=\
        serialport_global.h \
    serialport.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -lpthread libwsock32 libws2_32
