#-------------------------------------------------
#
# Project created by QtCreator 2019-12-27T15:45:47
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT += network
CONFIG += C++11

win32:CONFIG(release, debug|release): TARGET = ../../../../netWorkDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../netWorkDemo
else:unix:!macx: TARGET = ../../../netWorkDemo

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -ludp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -ludp
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -ludp

HEADERS +=
