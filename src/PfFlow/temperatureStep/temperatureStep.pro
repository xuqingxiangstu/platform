QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/temperatureStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/temperatureStep
else:unix:!macx: TARGET = ../../../stepLib/temperatureStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += TEMPERATURESTEP_LIBRARY

SOURCES += temperatureStep.cpp

HEADERS += temperatureStep.h\
    temperatureStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    
