#-------------------------------------------------
#
# Project created by QtCreator 2020-03-10T17:31:26
#
#-------------------------------------------------

QT       -= gui

QT       += sql
win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frame93
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frame93
else:unix:!macx: TARGET = ../../../frameLib/frame93
CONFIG += C++11
TEMPLATE = lib

DEFINES += FRAME93_LIBRARY

SOURCES += specialframe93.cpp

HEADERS += specialframe93.h\
        frame93_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
