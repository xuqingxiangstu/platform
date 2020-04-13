#-------------------------------------------------
#
# Project created by QtCreator 2020-02-18T10:37:09
#
#-------------------------------------------------

QT       -= gui
QT       += xlsx
QT       += sql
win32:CONFIG(release, debug|release): TARGET = ../../../../frameLib/generalFrame
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../frameLib/generalFrame
else:unix:!macx: TARGET = ../../../frameLib/generalFrame

TEMPLATE = lib
CONFIG += c++14

DEFINES += GENERALFRAME_LIBRARY

SOURCES += generalframe.cpp \
    protocolconfigure.cpp \
    infowordconf.cpp \
    infowordregion.cpp

HEADERS += generalframe.h\
        generalframe_global.h \
    protocolconfigure.h \
    infowordconf.h \
    infowordregion.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -ljsoncpp

 LIBS += -L$$OUT_PWD/../../../ -lsqlTable
