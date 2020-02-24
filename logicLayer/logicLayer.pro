#-------------------------------------------------
#
# Project created by QtCreator 2020-02-14T16:21:57
#
#-------------------------------------------------

QT       += core

QT       -= gui
CONFIG += c++11

win32:CONFIG(release, debug|release): TARGET =  ../../logicLayer
else:win32:CONFIG(debug, debug|release): TARGET =  ../../logicLayer
else:unix:!macx: TARGET =  ../logicLayer

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    rcvcmdtask.cpp \
    cmddecode.cpp

LIBS += -L$$OUT_PWD/../ -lrecordLog

LIBS += -L$$OUT_PWD/../stepLib/ -lRunFlow

LIBS += -L$$OUT_PWD/../ -lPfAdapterManager

LIBS += -L$$OUT_PWD/../adapterLib/ -lZmqRcvAdapter

LIBS += -L$$OUT_PWD/../adapterLib/ -lZmqSendAdapter

LIBS += -L$$OUT_PWD/../ -ljsoncpp

LIBS += -L$$OUT_PWD/../ -lcmdToJson

LIBS += -L$$OUT_PWD/../ -lzeroMq

HEADERS += \
    rcvcmdtask.h \
    cmddecode.h
