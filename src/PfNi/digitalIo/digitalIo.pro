#-------------------------------------------------
#
# Project created by QtCreator 2019-12-31T16:08:27
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../digitalIo
TEMPLATE = lib

CONFIG += C++11

DEFINES += DIGITALIO_LIBRARY

SOURCES += digitalio.cpp

HEADERS += digitalio.h\
        digitalio_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += $$PWD/../niDAQmx/NIDAQmx.lib
