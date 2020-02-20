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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -ludp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -ludp
else:unix:!macx:  LIBS += -L$$OUT_PWD/../../ -ludp

    
