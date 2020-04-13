#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T15:14:15
#
#-------------------------------------------------

QT       -= gui
CONFIG += c++14

win32:CONFIG(release, debug|release): TARGET = ../../../../icdFrameAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../icdFrameAdapter
else:unix:!macx: TARGET = ../../../icdFrameAdapter


TEMPLATE = lib

DEFINES += ICDFRAMEADAPTER_LIBRARY

SOURCES += icdframeadapter.cpp

HEADERS += icdframeadapter.h\
        icdframeadapter_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml

