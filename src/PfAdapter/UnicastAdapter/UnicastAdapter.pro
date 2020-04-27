QT       -= gui

QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/UnicastAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/UnicastAdapter
else:unix:!macx: TARGET = ../../../adapterLib/UnicastAdapter

TEMPLATE = lib

DEFINES += UNICASTADAPTER_LIBRARY
CONFIG += C++11
SOURCES += UnicastAdapter.cpp

HEADERS += UnicastAdapter.h\
      UnicastAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lTinyXml

LIBS += -L$$OUT_PWD/../../../ -ludp

LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
