QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/ZmqRcvAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/ZmqRcvAdapter
else:unix:!macx: TARGET = ../../../adapterLib/ZmqRcvAdapter

TEMPLATE = lib

DEFINES += ZMQRCVADAPTER_LIBRARY
CONFIG += c++11
SOURCES += ZmqRcvAdapter.cpp

HEADERS += ZmqRcvAdapter.h\
      ZmqRcvAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lzeroMq
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lzeroMq
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lzeroMq
    
    
win32{
    SRCFILE = $$OUT_PWD/../../../adapterLib/ZmqRcvAdapter.dll
    DSTFILE = $$OUT_PWD/../../../ZmqRcvAdapter.dll
    SRCDIR = $$replace(SRCFILE, /, \\)
    DSTDIR = $$replace(DSTFILE, /, \\)

    QMAKE_POST_LINK += copy $$SRCDIR $$DSTDIR
}
unix{
    SRCFILE = $$OUT_PWD/../../../adapterLib/libZmqRcvAdapter.so*
    DSTFILE = $$OUT_PWD/../../../

    QMAKE_POST_LINK += cp -d $$SRCFILE $$DSTFILE
}
