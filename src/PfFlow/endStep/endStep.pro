QT       -= gui

TARGET = ../../../../stepLib/endStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += ENDSTEP_LIBRARY

SOURCES += endStep.cpp

HEADERS += endStep.h\
    endStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

    
    
