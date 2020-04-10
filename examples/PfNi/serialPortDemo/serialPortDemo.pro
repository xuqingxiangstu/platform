#-------------------------------------------------
#
# Project created by QtCreator 2020-01-02T17:14:10
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = serialPortDemo
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lniSerialPort
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lniSerialPort

INCLUDEPATH += $$PWD/../../../src/PfNi/serialPort
DEPENDPATH += $$PWD/../../../src/PfNi/serialPort
