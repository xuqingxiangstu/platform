#-------------------------------------------------
#
# Project created by QtCreator 2020-02-13T09:37:29
#
#-------------------------------------------------

QT       += core

QT       -= gui
CONFIG += C++11

win32:CONFIG(release, debug|release): TARGET = ../../../../jsoncppDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../jsoncppDemo
else:unix:!macx: TARGET = ../../../jsoncppDemo

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -ljsoncpp


