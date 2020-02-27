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

LIBS += -L$$OUT_PWD/../../../ -ludp
    
