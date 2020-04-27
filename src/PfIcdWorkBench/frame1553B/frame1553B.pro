#-------------------------------------------------
#
# Project created by QtCreator 2020-04-26T09:37:47
#
#-------------------------------------------------

QT       -= gui

QT       += sql

win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frame1553B
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frame1553B
else:unix:!macx: TARGET = ../../../frameLib/frame1553B

CONFIG += c++11

TEMPLATE = lib

DEFINES += FRAME1553B_LIBRARY

SOURCES += frame1553b.cpp

HEADERS += frame1553b.h\
        frame1553b_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
