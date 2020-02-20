#-------------------------------------------------
#
# Project created by QtCreator 2019-12-27T15:43:28
#
#-------------------------------------------------

QT       -= gui
QT += core
QT += network

CONFIG += c++11

win32:CONFIG(release, debug|release): TARGET = ../../../../udp
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../udp
else:unix:!macx: TARGET = ../../../udp

TEMPLATE = lib

DEFINES += UDP_LIBRARY

SOURCES += \
    BroadcastUdp.cpp \
    MulticastUdp.cpp \
    UnicastUdp.cpp

HEADERS +=\
        udp_global.h \
    BroadcastUdp.h \
    MulticastUdp.h \
    UnicastUdp.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
