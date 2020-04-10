#-------------------------------------------------
#
# Project created by QtCreator 2019-12-31T17:03:12
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = digitalIoDemo
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -ldigitalIo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -ldigitalIo

INCLUDEPATH += $$PWD/../../../src/PfNi/digitalIo
DEPENDPATH += $$PWD/../../../src/PfNi/digitalIo
