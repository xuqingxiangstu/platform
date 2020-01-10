#-------------------------------------------------
#
# Project created by QtCreator 2020-01-02T15:17:26
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../niSerialPort
TEMPLATE = lib

DEFINES += SERIALPORT_LIBRARY
CONFIG += C++11
SOURCES += \
    niserialport.cpp

HEADERS +=\
    niserialport.h \
    niserialport_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += $$PWD/../niVisa/visa32.lib
