#-------------------------------------------------
#
# Project created by QtCreator 2020-01-02T10:52:53
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ../../../../analogInDemo
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lanalogIn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lanalogIn

INCLUDEPATH += $$PWD/../../../src/PfNi/analogIn
DEPENDPATH += $$PWD/../../../src/PfNi/analogIn

HEADERS +=
