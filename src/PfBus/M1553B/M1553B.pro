#-------------------------------------------------
#
# Project created by QtCreator 2020-04-24T16:49:54
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib

CONFIG += c++11

win32:CONFIG(release, debug|release): TARGET = ../../../../M1553B
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../M1553B
else:unix:!macx: TARGET = ../../../M1553B

DEFINES += M1553B_LIBRARY

SOURCES += m1553b.cpp

HEADERS += m1553b.h\
        m1553b_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
win32:CONFIG(release, debug|release): LIBS += $$PWD/ZHHK1553/windows/ZHHK1553.lib
else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/ZHHK1553/windows/ZHHK1553.lib
else:unix:!macx: LIBS += -L$$PWD/ZHHK1553/linux/ -lZHHK1553

