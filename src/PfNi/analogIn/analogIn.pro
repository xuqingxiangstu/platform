#-------------------------------------------------
#
# Project created by QtCreator 2020-01-02T09:33:59
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../analogIn
TEMPLATE = lib

CONFIG += C++11

DEFINES += ANALOGIN_LIBRARY

SOURCES += analogin.cpp

HEADERS += analogin.h\
        analogin_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += $$PWD/../niDAQmx/NIDAQmx.lib
