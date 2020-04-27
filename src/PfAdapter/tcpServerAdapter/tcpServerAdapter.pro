#-------------------------------------------------
#
# Project created by QtCreator 2020-04-22T10:29:01
#
#-------------------------------------------------

QT       -= gui

QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/tcpServerAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/tcpServerAdapter
else:unix:!macx: TARGET = ../../../adapterLib/tcpServerAdapter
TEMPLATE = lib

CONFIG += c++11

DEFINES += TCPSERVERADAPTER_LIBRARY

SOURCES += tcpserveradapter.cpp

HEADERS += tcpserveradapter.h\
        tcpserveradapter_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
LIBS += -L$$OUT_PWD/../../../ -ltcpServer
LIBS += -L$$OUT_PWD/../../../ -lTinyXml
