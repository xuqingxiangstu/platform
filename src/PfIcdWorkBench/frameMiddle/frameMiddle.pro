#-------------------------------------------------
#
# Project created by QtCreator 2020-04-13T13:07:42
#
#-------------------------------------------------

QT       -= gui
QT       += sql
win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameMiddle
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameMiddle
else:unix:!macx: TARGET = ../../../frameLib/frameMiddle
CONFIG += c++11

TEMPLATE = lib

DEFINES += FRAMEMIDDLE_LIBRARY

SOURCES += framemiddle.cpp

HEADERS += framemiddle.h\
        framemiddle_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp

LIBS += -L$$OUT_PWD/../../../ -lframeNumber
