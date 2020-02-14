QT       -= gui


win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/setSwitchOutStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/setSwitchOutStep
else:unix:!macx: TARGET = ../../../stepLib/setSwitchOutStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += SETSWITCHOUTSTEP_LIBRARY

SOURCES += setSwitchOutStep.cpp

HEADERS += setSwitchOutStep.h\
    setSwitchOutStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    
