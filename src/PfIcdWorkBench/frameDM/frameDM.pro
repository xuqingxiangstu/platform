#-------------------------------------------------
#
# Project created by QtCreator 2020-04-18T17:43:57
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/frameDM
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/frameDM
else:unix:!macx: TARGET = ../../../frameLib/frameDM
CONFIG += c++11

TEMPLATE = lib

DEFINES += FRAMEDM_LIBRARY

SOURCES += framedm.cpp

HEADERS += framedm.h\
        framedm_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}



LIBS += -L$$OUT_PWD/../../../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -licdData

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
