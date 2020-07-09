#-------------------------------------------------
#
# Project created by QtCreator 2020-03-10T18:13:40
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TEMPLATE = lib

CONFIG += C++11

win32:CONFIG(release, debug|release): TARGET = ../../../../sqlTable
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../sqlTable
else:unix:!macx: TARGET = ../../../sqlTable

DEFINES += PARAMSTABLE_LIBRARY

SOURCES += paramstable.cpp \
    sqltable.cpp \
    flowrecordtable.cpp \
    udptable.cpp \
    sysinterfacetable.cpp \
    systemtable.cpp \
    tcptable.cpp \
    tcpservertable.cpp \
    m1553btable.cpp

HEADERS += paramstable.h\
        paramstable_global.h \
    sqltable.h \
    flowrecordtable.h \
    udptable.h \
    sysinterfacetable.h \
    systemtable.h \
    tcptable.h \
    tcpservertable.h \
    m1553btable.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include($$PWD/parseResult/parseResult.pri)

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
