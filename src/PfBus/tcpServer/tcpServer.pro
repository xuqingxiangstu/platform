#-------------------------------------------------
#
# Project created by QtCreator 2020-04-17T09:46:29
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib

QT += network

CONFIG += c++11

win32:CONFIG(release, debug|release): TARGET = ../../../../tcpServer
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../tcpServer
else:unix:!macx: TARGET = ../../../tcpServer

DEFINES += TCPSERVER_LIBRARY

SOURCES += tcpserver.cpp

HEADERS += tcpserver.h\
        tcpserver_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
