#-------------------------------------------------
#
# Project created by QtCreator 2019-12-31T15:08:05
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../dynCreate
TEMPLATE = lib

DEFINES += DYNCREATE_LIBRARY

SOURCES += \
    dyncreate.cpp

HEADERS +=\
        dyncreate_global.h \
    dyncreate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
