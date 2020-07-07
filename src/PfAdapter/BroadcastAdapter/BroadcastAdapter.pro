QT       -= gui
QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/BroadcastAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/BroadcastAdapter
else:unix:!macx: TARGET = ../../../adapterLib/BroadcastAdapter

TEMPLATE = lib

DEFINES += BROADCASTADAPTER_LIBRARY
CONFIG += C++11
SOURCES += BroadcastAdapter.cpp

HEADERS += BroadcastAdapter.h\
      BroadcastAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lTinyXml
LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
LIBS += -L$$OUT_PWD/../../../ -ludp
    
win32{
    SRCFILE = $$OUT_PWD/../../../adapterLib/BroadcastAdapter.dll
    DSTFILE = $$OUT_PWD/../../../BroadcastAdapter.dll
    SRCDIR = $$replace(SRCFILE, /, \\)
    DSTDIR = $$replace(DSTFILE, /, \\)

    QMAKE_POST_LINK += copy $$SRCDIR $$DSTDIR
}
unix{
    SRCFILE = $$OUT_PWD/../../../adapterLib/libBroadcastAdapter.so*
    DSTFILE = $$OUT_PWD/../../../

    QMAKE_POST_LINK += cp -d $$SRCFILE $$DSTFILE
}
