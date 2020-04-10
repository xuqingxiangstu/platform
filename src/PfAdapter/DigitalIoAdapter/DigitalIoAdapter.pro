QT       -= gui

TARGET = ../../../../adapterLib/DigitalIoAdapter
TEMPLATE = lib

DEFINES += DIGITALIOADAPTER_LIBRARY
CONFIG += C++11
SOURCES += DigitalIoAdapter.cpp

HEADERS += DigitalIoAdapter.h\
      DigitalIoAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -ldigitalIo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -ldigitalIo
