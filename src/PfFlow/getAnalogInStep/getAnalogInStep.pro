QT       -= gui

TARGET = ../../../../stepLib/getAnalogInStep
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lPfAdapter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lPfAdapter

    
