#-------------------------------------------------
#
# Project created by QtCreator 2020-04-16T16:09:41
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameXK
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameXK
else:unix:!macx: TARGET = ../../../frameLib/frameXK
CONFIG += c++11


TEMPLATE = lib

DEFINES += FRAMEXK_LIBRARY

SOURCES += framexk.cpp

HEADERS += framexk.h\
        framexk_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
