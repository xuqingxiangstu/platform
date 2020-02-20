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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -ludp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -ludp
else:unix:!macx:  LIBS += -L$$OUT_PWD/../../ -ludp
