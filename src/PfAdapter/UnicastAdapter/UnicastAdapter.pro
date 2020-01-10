QT       -= gui

TARGET = ../../../../adapterLib/UnicastAdapter
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
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lnetWork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lnetWork
