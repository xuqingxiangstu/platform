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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -ludp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -ludp
else:unix:!macx:  LIBS += -L$$OUT_PWD/../../ -ludp

