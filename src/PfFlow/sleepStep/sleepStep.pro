QT       -= gui


win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/sleepStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/sleepStep
else:unix:!macx: TARGET = ../../../stepLib/sleepStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += SLEEPSTEP_LIBRARY

SOURCES += sleepStep.cpp

HEADERS += sleepStep.h\
    sleepStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    
