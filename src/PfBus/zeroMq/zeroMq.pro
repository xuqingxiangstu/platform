#-------------------------------------------------
#
# Project created by QtCreator 2019-12-30T16:23:58
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../zeroMq
TEMPLATE = lib

CONFIG += C++11

DEFINES += ZEROMQ_LIBRARY

SOURCES += \
    zmqsend.cpp \
    zmqreceive.cpp

HEADERS +=\
        zeromq_global.h \
    zmqsend.h \
    zmqreceive.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += $$PWD/zmq/libzmq.lib
