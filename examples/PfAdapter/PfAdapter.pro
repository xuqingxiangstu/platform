#-------------------------------------------------
#
# Project created by QtCreator 2020-01-03T16:40:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ../../../PfAdapterDemo
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++14
TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lPfAdapterManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lPfAdapterManager

INCLUDEPATH += $$PWD/../../src/PfAdapter
DEPENDPATH += $$PWD/../../src/PfAdapter

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lanalogIn
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lanalogIn

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lnetWork
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lnetWork

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lserialPort
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lserialPort
