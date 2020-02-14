#-------------------------------------------------
#
# Project created by QtCreator 2020-02-14T14:22:55
#
#-------------------------------------------------

QT       += core

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../tinyxmlDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../tinyxmlDemo
else:unix:!macx: TARGET = ../../../tinyxmlDemo

CONFIG += C++11

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
