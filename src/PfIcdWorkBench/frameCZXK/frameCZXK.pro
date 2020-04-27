#-------------------------------------------------
#
# Project created by QtCreator 2020-04-19T14:28:43
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameCZXK
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameCZXK
else:unix:!macx: TARGET = ../../../frameLib/frameCZXK
CONFIG += c++11


TEMPLATE = lib

DEFINES += FRAMECZXK_LIBRARY

SOURCES += frameczxk.cpp

HEADERS += frameczxk.h\
        frameczxk_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
