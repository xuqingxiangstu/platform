#-------------------------------------------------
#
# Project created by QtCreator 2020-01-02T10:21:24
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../analogOut
TEMPLATE = lib

CONFIG += C++11

DEFINES += ANALOGOUT_LIBRARY

SOURCES += analogout.cpp

HEADERS += analogout.h\
        analogout_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += $$PWD/../niDAQmx/NIDAQmx.lib
