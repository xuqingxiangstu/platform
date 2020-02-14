QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/endStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/endStep
else:unix:!macx: TARGET = ../../../stepLib/endStep

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

    
    
