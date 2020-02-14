#-------------------------------------------------
#
# Project created by QtCreator 2020-02-14T15:21:42
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT += xlsx
CONFIG += C++11

win32:CONFIG(release, debug|release): TARGET = ../../../../xlsxDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../xlsxDemo
else:unix:!macx: TARGET = ../../../xlsxDemo

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
