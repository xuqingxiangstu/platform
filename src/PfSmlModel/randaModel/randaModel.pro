#-------------------------------------------------
#
# Project created by QtCreator 2020-02-11T09:58:28
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../algorithmLib/randaModel
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../algorithmLib/randaModel
else:unix:!macx: TARGET = ../../../algorithmLib/randaModel

TEMPLATE = lib

DEFINES += RANDAMODEL_LIBRARY
CONFIG += C++11
SOURCES += randamodel.cpp

HEADERS += randamodel.h\
        randamodel_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
