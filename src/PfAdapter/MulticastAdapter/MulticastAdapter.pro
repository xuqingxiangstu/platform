QT       -= gui

QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/MulticastAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/MulticastAdapter
else:unix:!macx: TARGET = ../../../adapterLib/MulticastAdapter

TEMPLATE = lib

DEFINES += MULTICASTADAPTER_LIBRARY
CONFIG += C++11
SOURCES += MulticastAdapter.cpp

HEADERS += MulticastAdapter.h\
      MulticastAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lTinyXml

LIBS += -L$$OUT_PWD/../../../ -ludp
