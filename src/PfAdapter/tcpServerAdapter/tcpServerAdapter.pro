#-------------------------------------------------
#
# Project created by QtCreator 2020-04-22T10:29:01
#
#-------------------------------------------------

QT       -= gui

QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/tcpServerAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/tcpServerAdapter
else:unix:!macx: TARGET = ../../../adapterLib/tcpServerAdapter
TEMPLATE = lib

CONFIG += c++11

DEFINES += TCPSERVERADAPTER_LIBRARY

SOURCES += tcpserveradapter.cpp

HEADERS += tcpserveradapter.h\
        tcpserveradapter_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
LIBS += -L$$OUT_PWD/../../../ -ltcpServer
LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32{
    SRCFILE = $$OUT_PWD/../../../adapterLib/tcpServerAdapter.dll
    DSTFILE = $$OUT_PWD/../../../tcpServerAdapter.dll
    SRCDIR = $$replace(SRCFILE, /, \\)
    DSTDIR = $$replace(DSTFILE, /, \\)

    QMAKE_POST_LINK += copy $$SRCDIR $$DSTDIR
}
unix{
    SRCFILE = $$OUT_PWD/../../../adapterLib/libtcpServerAdapter.so*
    DSTFILE = $$OUT_PWD/../../../

    QMAKE_POST_LINK += cp -d $$SRCFILE $$DSTFILE
}


