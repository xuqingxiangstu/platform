#-------------------------------------------------
#
# Project created by QtCreator 2020-07-09T12:23:53
#
#-------------------------------------------------

QT       += core

QT       -= gui

CONFIG += c++14

TARGET = ../../mulitThreadSqlite
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
LIBS += -L$$OUT_PWD/../ -ljsoncpp
LIBS += -L$$OUT_PWD/../ -lsqlTable
