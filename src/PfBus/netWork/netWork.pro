#-------------------------------------------------
#
# Project created by QtCreator 2019-12-27T15:43:28
#
#-------------------------------------------------

QT       -= gui
CONFIG += c++11
TARGET = ../../../../netWork
TEMPLATE = lib

DEFINES += NETWORK_LIBRARY

SOURCES += \
    BroadcastUdp.cpp \
    MulticastUdp.cpp \
    UnicastUdp.cpp

HEADERS +=\
        network_global.h \
    BroadcastUdp.h \
    MulticastUdp.h \
    UnicastUdp.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
LIBS += -lpthread libwsock32 libws2_32
