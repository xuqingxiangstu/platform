#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T15:38:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:CONFIG(release, debug|release): TARGET = ../../../createStepTemplate
else:win32:CONFIG(debug, debug|release): TARGET = ../../../createStepTemplate
else:unix:!macx: TARGET = ../../createStepTemplate

TEMPLATE = app
CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
