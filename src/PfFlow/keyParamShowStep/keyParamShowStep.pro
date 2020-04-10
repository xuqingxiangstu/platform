QT       -= gui

TARGET = ../../../../stepLib/keyParamShowStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += KEYPARAMSHOWSTEP_LIBRARY

SOURCES += keyParamShowStep.cpp

HEADERS += keyParamShowStep.h\
    keyParamShowStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    