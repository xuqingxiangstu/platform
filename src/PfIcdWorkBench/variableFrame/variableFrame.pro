#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T11:06:39
#
#-------------------------------------------------

QT       -= gui
QT       += xlsx


win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/variableFrame
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/variableFrame
else:unix:!macx: TARGET = ../../../frameLib/variableFrame

TEMPLATE = lib


CONFIG += C++14

DEFINES += VARIABLEFRAME_LIBRARY

SOURCES += variableframe.cpp \
    protocolconfigure.cpp \
    subprotocolconfigure.cpp

HEADERS += variableframe.h\
        variableframe_global.h \
    protocolconfigure.h \
    subprotocolconfigure.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -licdData
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -licdData
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -licdData

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcrc
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lcrc
