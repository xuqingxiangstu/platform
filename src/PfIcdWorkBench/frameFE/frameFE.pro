#-------------------------------------------------
#
# Project created by QtCreator 2020-03-11T10:54:17
#
#-------------------------------------------------

QT       -= gui

QT       += sql
win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameFE
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameFE
else:unix:!macx: TARGET = ../../../frameLib/frameFE
CONFIG += C++11

TEMPLATE = lib

DEFINES += FRAMEFE_LIBRARY

SOURCES += framefe.cpp

HEADERS += framefe.h\
        framefe_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
