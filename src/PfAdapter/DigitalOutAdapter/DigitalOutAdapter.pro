QT       -= gui

TARGET = ../../../../adapterLib/DigitalOutAdapter
TEMPLATE = lib

DEFINES += DIGITALOUTADAPTER_LIBRARY
CONFIG += C++11
SOURCES += DigitalOutAdapter.cpp

HEADERS += DigitalOutAdapter.h\
      DigitalOutAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
   In
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -ldigitalIo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -ldigitalIo
