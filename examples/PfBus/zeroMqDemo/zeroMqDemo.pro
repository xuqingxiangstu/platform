#-------------------------------------------------
#
# Project created by QtCreator 2019-12-30T18:00:56
#
#-------------------------------------------------

QT       += core

QT       -= gui
CONFIG += C++11

win32:CONFIG(release, debug|release): TARGET = ../../../../zeroMqDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../zeroMqDemo
else:unix:!macx: TARGET = ../../../zeroMqDemo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lzeroMq
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lzeroMq
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lzeroMq

INCLUDEPATH += $$PWD/../../../src/PfBus/zeroMq
DEPENDPATH += $$PWD/../../../src/PfBus/zeroMq

unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lzmq
