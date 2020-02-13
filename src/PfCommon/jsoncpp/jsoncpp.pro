#-------------------------------------------------
#
# Project created by QtCreator 2020-02-13T09:32:50
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../jsoncpp
TEMPLATE = lib
CONFIG += C++11
DEFINES += JSONCPP_LIBRARY

SOURCES += \
    jsoncpp.cpp

HEADERS += \
    json.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
