QT       -= gui

TARGET = ../../../../adapterLib/AnalogInAdapter
TEMPLATE = lib

DEFINES += ANALOGINADAPTER_LIBRARY
CONFIG += C++11
SOURCES += AnalogInAdapter.cpp

HEADERS += AnalogInAdapter.h\
      AnalogInAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lanalogIn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lanalogIn
    
    
