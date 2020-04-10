#-------------------------------------------------
#
# Project created by QtCreator 2020-01-07T15:42:56
#
#-------------------------------------------------

QT       += core

QT       -= gui
win32:CONFIG(release, debug|release): TARGET = ../../../../recordLogDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../recordLogDemo
else:unix:!macx: TARGET = ../../../recordLogDemo

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lrecordLog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lrecordLog
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lrecordLog

HEADERS += \
    recordlogdemo.h
