QT       -= gui

TARGET = ../../../../stepLib/continueStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += CONTINUESTEP_LIBRARY

SOURCES += continueStep.cpp

HEADERS += continueStep.h\
    continueStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    