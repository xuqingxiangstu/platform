#-------------------------------------------------
#
# Project created by QtCreator 2019-12-31T17:02:59
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ../../../../relayCtrlDemo
CONFIG += C++11
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lrelayCtrl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lrelayCtrl

INCLUDEPATH += $$PWD/../../../src/PfNi/relayCtrl
DEPENDPATH += $$PWD/../../../src/PfNi/relayCtrl
