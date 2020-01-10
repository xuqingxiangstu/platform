#-------------------------------------------------
#
# Project created by QtCreator 2019-12-27T15:45:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = netWorkDemo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    udpTest.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lnetWork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lnetWork

INCLUDEPATH += $$PWD/../../../src/PfBus/netWork
DEPENDPATH += $$PWD/../../../src/PfBus/netWork

HEADERS += \
    udpTest.h
