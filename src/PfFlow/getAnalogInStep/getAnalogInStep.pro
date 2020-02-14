QT       -= gui


win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/getAnalogInStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/getAnalogInStep
else:unix:!macx: TARGET = ../../../stepLib/getAnalogInStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += GETANALOGINSTEP_LIBRARY

SOURCES += getAnalogInStep.cpp

HEADERS += getAnalogInStep.h\
    getAnalogInStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lPfAdapterManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lPfAdapterManager
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lPfAdapterManager

    
