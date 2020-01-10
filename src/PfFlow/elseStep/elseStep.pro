QT       -= gui

TARGET = ../../../../stepLib/elseStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += ELSESTEP_LIBRARY

SOURCES += elseStep.cpp

HEADERS += elseStep.h\
    elseStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    