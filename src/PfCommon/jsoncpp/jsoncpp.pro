#-------------------------------------------------
#
# Project created by QtCreator 2020-02-13T09:32:50
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../jsoncpp
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../jsoncpp
else:unix:!macx: TARGET = ../../../jsoncpp


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
