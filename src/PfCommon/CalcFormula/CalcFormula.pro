#-------------------------------------------------
#
# Project created by QtCreator 2020-02-13T14:52:50
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../CalcFormula
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../CalcFormula
else:unix:!macx: TARGET = ../../../CalcFormula

TEMPLATE = lib

DEFINES += CALCFORMULA_LIBRARY

SOURCES += calcformula.cpp

HEADERS += calcformula.h\
        calcformula_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
