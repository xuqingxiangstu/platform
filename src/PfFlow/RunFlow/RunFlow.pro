#-------------------------------------------------
#
# Project created by QtCreator 2020-02-11T09:33:50
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../RunFlow
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../RunFlow
else:unix:!macx: TARGET = ../../../RunFlow

TEMPLATE = lib
CONFIG += C++11

DEFINES += RUNFLOW_LIBRARY

SOURCES += runflow.cpp \
    autorun.cpp \
    runcase.cpp \
    runprocedure.cpp

HEADERS += runflow.h\
        runflow_global.h \
    runprocedure.h \
    autorun.h \
    runcase.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lTinyXml

LIBS += -L$$OUT_PWD/../../../ -lcmdToJson
