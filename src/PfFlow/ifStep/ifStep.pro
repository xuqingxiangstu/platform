#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T09:44:27
#
#-------------------------------------------------

QT       -= gui

TARGET = ../../../../stepLib/ifStep
TEMPLATE = lib
CONFIG += C++11
DEFINES += IFSTEP_LIBRARY

SOURCES += ifstep.cpp

HEADERS += ifstep.h\
        ifstep_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcondition
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcondition
