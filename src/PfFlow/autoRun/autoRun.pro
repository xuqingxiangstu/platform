#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T09:37:09
#
#-------------------------------------------------

QT       += core

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../autoRun
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../autoRun
else:unix:!macx: TARGET = ../../../autoRun

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp \
    autorun.cpp \
    flow.cpp \
    runcase.cpp \
    runprocedure.cpp

HEADERS += \
    autorun.h \
    flow.h \
    runcase.h \
    runprocedure.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
