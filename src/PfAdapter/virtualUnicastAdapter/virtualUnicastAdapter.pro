QT       -= gui

QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/virtualUnicastAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/virtualUnicastAdapter
else:unix:!macx: TARGET = ../../../adapterLib/virtualUnicastAdapter
TEMPLATE = lib

DEFINES += VIRTUALUNICASTADAPTER_LIBRARY
CONFIG += C++11
SOURCES += virtualUnicastAdapter.cpp

HEADERS += virtualUnicastAdapter.h\
      virtualUnicastAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
LIBS += -L$$OUT_PWD/../../../ -ljsoncpp

LIBS += -L$$OUT_PWD/../../../ -ludp

    
