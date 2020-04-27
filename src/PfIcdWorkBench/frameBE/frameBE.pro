#-------------------------------------------------
#
# Project created by QtCreator 2020-04-15T14:27:01
#
#-------------------------------------------------

QT       -= gui
QT       += sql
win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameBE
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameBE
else:unix:!macx: TARGET = ../../../frameLib/frameBE
CONFIG += c++11

TEMPLATE = lib

DEFINES += FRAMEBE_LIBRARY

SOURCES += framebe.cpp

HEADERS += framebe.h\
        framebe_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
