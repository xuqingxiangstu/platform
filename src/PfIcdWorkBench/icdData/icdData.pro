#-------------------------------------------------
#
# Project created by QtCreator 2020-02-11T10:05:12
#
#-------------------------------------------------

QT       -= gui
CONFIG += C++14
QT       += xlsx
TARGET = ../../../../icdData
TEMPLATE = lib

DEFINES += ICDDATA_LIBRARY

SOURCES += \
    datacalc.cpp \
    dataconvert.cpp \
    datastorage.cpp \
    dataFrame.cpp

HEADERS +=\
        icddata_global.h \
    datacalc.h \
    dataconvert.h \
    datastorage.h \
    datatype.h \
    dataFrame.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
