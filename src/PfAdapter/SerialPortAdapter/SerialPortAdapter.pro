QT       -= gui

TARGET = ../../../../adapterLib/SerialPortAdapter
TEMPLATE = lib

DEFINES += SERIALPORTADAPTER_LIBRARY
CONFIG += C++11
SOURCES += SerialPortAdapter.cpp

HEADERS += SerialPortAdapter.h\
      SerialPortAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lserialPort
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lserialPort
    
    
