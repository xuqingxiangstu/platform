QT       -= gui

TARGET = ../../../../stepLib/revUdpStep
win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/revUdpStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/revUdpStep
else:unix:!macx: TARGET = ../../../stepLib/revUdpStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += REVUDPSTEP_LIBRARY

SOURCES += revUdpStep.cpp

HEADERS += revUdpStep.h\
    revUdpStep_global.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -licdFrameAdapter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -licdFrameAdapter
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -licdFrameAdapter
    
    
