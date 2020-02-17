#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T16:52:28
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../modelManager
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../modelManager
else:unix:!macx: TARGET = ../../../modelManager

TEMPLATE = lib
CONFIG += C++11
DEFINES += MODELMANAGER_LIBRARY

SOURCES += modelmanager.cpp

HEADERS += modelmanager.h\
        modelmanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
