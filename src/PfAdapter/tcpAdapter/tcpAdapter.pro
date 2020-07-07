QT       -= gui

QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/tcpAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/tcpAdapter
else:unix:!macx: TARGET = ../../../adapterLib/tcpAdapter
TEMPLATE = lib

DEFINES += TCPADAPTER_LIBRARY
CONFIG += c++11
SOURCES += tcpAdapter.cpp

HEADERS += tcpAdapter.h\
      tcpAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
    
LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
LIBS += -L$$OUT_PWD/../../../ -ltcpClient

win32{
    SRCFILE = $$OUT_PWD/../../../adapterLib/tcpAdapter.dll
    DSTFILE = $$OUT_PWD/../../../tcpAdapter.dll
    SRCDIR = $$replace(SRCFILE, /, \\)
    DSTDIR = $$replace(DSTFILE, /, \\)

    QMAKE_POST_LINK += copy $$SRCDIR $$DSTDIR
}
unix{
    SRCFILE = $$OUT_PWD/../../../adapterLib/libtcpAdapter.so*
    DSTFILE = $$OUT_PWD/../../../

    QMAKE_POST_LINK += cp -d $$SRCFILE $$DSTFILE
}

