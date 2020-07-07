#-------------------------------------------------
#
# Project created by QtCreator 2020-04-24T17:01:26
#
#-------------------------------------------------

QT       -= gui

TARGET = m1553Adapter

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/m1553Adapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/m1553Adapter
else:unix:!macx: TARGET = ../../../adapterLib/m1553Adapter

TEMPLATE = lib
CONFIG += c++11
DEFINES += M1553ADAPTER_LIBRARY

SOURCES += m1553adapter.cpp

HEADERS += m1553adapter.h\
        m1553adapter_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


LIBS += -L$$OUT_PWD/../../../ -lTinyXml
LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
LIBS += -L$$OUT_PWD/../../../ -lM1553B

win32{
    SRCFILE = $$OUT_PWD/../../../adapterLib/m1553Adapter.dll
    DSTFILE = $$OUT_PWD/../../../m1553Adapter.dll
    SRCDIR = $$replace(SRCFILE, /, \\)
    DSTDIR = $$replace(DSTFILE, /, \\)

    QMAKE_POST_LINK += copy $$SRCDIR $$DSTDIR
}
unix{
    SRCFILE = $$OUT_PWD/../../../adapterLib/libm1553Adapter.so*
    DSTFILE = $$OUT_PWD/../../../

    QMAKE_POST_LINK += cp -d $$SRCFILE $$DSTFILE
}
