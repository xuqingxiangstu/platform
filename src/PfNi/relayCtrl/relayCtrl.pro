#-------------------------------------------------
#
# Project created by QtCreator 2019-12-31T15:30:10
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../relayCtrl
TEMPLATE = lib

CONFIG += C++11

DEFINES += RELAYCTRL_LIBRARY

SOURCES += relayctrl.cpp

HEADERS += relayctrl.h\
        relayctrl_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += $$PWD/../niSwitch/niswitch.lib
