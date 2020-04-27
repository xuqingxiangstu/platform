#-------------------------------------------------
#
# Project created by QtCreator 2020-04-17T09:46:29
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib

QT += network

CONFIG += c++11

win32:CONFIG(release, debug|release): TARGET = ../../../../tcpClient
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../tcpClient
else:unix:!macx: TARGET = ../../../tcpClient

DEFINES += TCPCLIENT_LIBRARY

SOURCES += tcpclient.cpp

HEADERS += tcpclient.h\
        tcpclient_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
