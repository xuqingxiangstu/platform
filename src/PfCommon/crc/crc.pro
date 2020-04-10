#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T13:36:18
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../crc
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../crc
else:unix:!macx: TARGET = ../../../crc

TEMPLATE = lib

DEFINES += CRC_LIBRARY

CONFIG += C++11

SOURCES += crc.cpp

HEADERS += crc.h\
        crc_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
