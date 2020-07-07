#-------------------------------------------------
#
# Project created by QtCreator 2020-06-13T11:01:08
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameJG
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameJG
else:unix:!macx: TARGET = ../../../frameLib/frameJG
CONFIG += c++11


TEMPLATE = lib

DEFINES += FRAMEJG_LIBRARY

SOURCES += framejg.cpp

HEADERS += framejg.h\
        framejg_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp

LIBS += -L$$OUT_PWD/../../../ -lframeNumber
