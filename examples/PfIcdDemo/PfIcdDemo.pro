#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T15:41:41
#
#-------------------------------------------------

QT       += core

QT       -= gui
CONFIG += C++11
TARGET = ../../../PfIcdDemo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -licdFrameAdapter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -licdFrameAdapter

