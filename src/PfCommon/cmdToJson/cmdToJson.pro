#-------------------------------------------------
#
# Project created by QtCreator 2020-02-20T12:18:09
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../cmdToJson
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../cmdToJson
else:unix:!macx: TARGET = ../../../cmdToJson

TEMPLATE = lib
CONFIG += C++11
DEFINES += CMDTOJSON_LIBRARY

SOURCES += cmdtojson.cpp

HEADERS += cmdtojson.h\
        cmdtojson_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
