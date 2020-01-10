QT       -= gui

TARGET = ../../../../stepLib/printfStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += PRINTFSTEP_LIBRARY

SOURCES += printfStep.cpp

HEADERS += printfStep.h\
    printfStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcondition
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcondition
