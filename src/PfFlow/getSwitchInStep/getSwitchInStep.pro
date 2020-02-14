QT       -= gui


win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/getSwitchInStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/getSwitchInStep
else:unix:!macx: TARGET = ../../../stepLib/getSwitchInStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += GETSWITCHINSTEP_LIBRARY

SOURCES += getSwitchInStep.cpp

HEADERS += getSwitchInStep.h\
    getSwitchInStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    
