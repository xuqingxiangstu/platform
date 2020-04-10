#-------------------------------------------------
#
# Project created by QtCreator 2020-01-03T16:40:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../PfAdapterDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../PfAdapterDemo
else:unix:!macx: TARGET = ../../PfAdapterDemo

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++14
TEMPLATE = app


SOURCES += main.cpp

LIBS += -L$$OUT_PWD/../../ -lPfAdapterManager

LIBS += -L$$OUT_PWD/../../ -lTinyXml

INCLUDEPATH += $$PWD/../../src/PfAdapter
DEPENDPATH += $$PWD/../../src/PfAdapter
