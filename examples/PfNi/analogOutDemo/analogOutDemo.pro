#-------------------------------------------------
#
# Project created by QtCreator 2020-01-02T10:53:03
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ../../../../analogOutDemo
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lanalogOut
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lanalogOut

INCLUDEPATH += $$PWD/../../../src/PfNi/analogOut
DEPENDPATH += $$PWD/../../../src/PfNi/analogOut
