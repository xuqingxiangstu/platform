#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T16:19:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


win32:CONFIG(release, debug|release): TARGET = ../../../createAdapterTemplate
else:win32:CONFIG(debug, debug|release): TARGET = ../../../createAdapterTemplate
else:unix:!macx: TARGET = ../../createAdapterTemplate

TEMPLATE = app

CONFIG += C++11
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
