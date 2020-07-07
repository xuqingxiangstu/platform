#-------------------------------------------------
#
# Project created by QtCreator 2019-12-30T16:23:58
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../zeroMq
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../zeroMq
else:unix:!macx: TARGET = ../../../zeroMq

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

win32:CONFIG(release, debug|release): LIBS += $$PWD/zmq/libzmq.lib
else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/zmq/libzmq.lib
else:unix:!macx: LIBS += $$PWD/zmq/libzmq.so
