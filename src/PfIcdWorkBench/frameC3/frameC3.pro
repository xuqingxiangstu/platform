#-------------------------------------------------
#
# Project created by QtCreator 2020-04-22T09:25:46
#
#-------------------------------------------------

QT       -= gui



win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameC3
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameC3
else:unix:!macx: TARGET = ../../../frameLib/frameC3
CONFIG += c++11

TEMPLATE = lib

DEFINES += FRAMEC3_LIBRARY

SOURCES += framec3.cpp

HEADERS += framec3.h\
        framec3_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
