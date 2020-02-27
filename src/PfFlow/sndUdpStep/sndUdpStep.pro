QT       -= gui

TARGET = ../../../../stepLib/sndUdpStep
win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/sndUdpStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/sndUdpStep
else:unix:!macx: TARGET = ../../../stepLib/sndUdpStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += SNDUDPSTEP_LIBRARY

SOURCES += sndUdpStep.cpp

HEADERS += sndUdpStep.h\
    sndUdpStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    