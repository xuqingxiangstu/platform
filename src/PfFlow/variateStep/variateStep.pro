QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/variateStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/variateStep
else:unix:!macx: TARGET = ../../../stepLib/variateStep


TEMPLATE = lib
CONFIG += C++11
DEFINES += VARIATESTEP_LIBRARY

SOURCES += variateStep.cpp

HEADERS += variateStep.h\
    variateStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lTinyXml

LIBS += -L$$OUT_PWD/../../../ -lcondition

    
    
