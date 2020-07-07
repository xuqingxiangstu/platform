#-------------------------------------------------
#
# Project created by QtCreator 2020-04-30T09:10:03
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../frameNumber
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameNumber
else:unix:!macx: TARGET = ../../../frameNumber
CONFIG += c++11

TEMPLATE = lib

DEFINES += FRAMENUMBER_LIBRARY

SOURCES += framenumber.cpp

HEADERS += framenumber.h\
        framenumber_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
