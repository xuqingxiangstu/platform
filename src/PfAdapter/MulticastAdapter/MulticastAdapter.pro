QT       -= gui

TARGET = ../../../../adapterLib/MulticastAdapter
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
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lnetWork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lnetWork
