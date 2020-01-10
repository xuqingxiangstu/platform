QT       -= gui

TARGET = ../../../../stepLib/setAnalogOutStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += SETANALOGOUTSTEP_LIBRARY

SOURCES += setAnalogOutStep.cpp

HEADERS += setAnalogOutStep.h\
    setAnalogOutStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    