#-------------------------------------------------
#
# Project created by QtCreator 2019-12-27T15:45:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../netWorkDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../netWorkDemo
else:unix:!macx: TARGET = ../../../netWorkDemo

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    udpTest.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -ludp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -ludp
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -ludp

INCLUDEPATH += $$PWD/../../../src/PfBus/udp
DEPENDPATH += $$PWD/../../../src/PfBus/udp

HEADERS += \
    udpTest.h
