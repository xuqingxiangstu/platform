#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T16:45:35
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../PfAdapterManager
TEMPLATE = lib
CONFIG += C++11
DEFINES += PFADAPTERMANAGER_LIBRARY

SOURCES += pfadaptermanager.cpp

HEADERS += pfadaptermanager.h\
        pfadaptermanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
