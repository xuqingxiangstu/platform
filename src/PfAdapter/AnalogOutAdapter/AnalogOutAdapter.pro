QT       -= gui

TARGET = ../../../../adapterLib/AnalogOutAdapter
TEMPLATE = lib

DEFINES += ANALOGOUTADAPTER_LIBRARY
CONFIG += C++11
SOURCES += AnalogOutAdapter.cpp

HEADERS += AnalogOutAdapter.h\
      AnalogOutAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lanalogOut
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lanalogOut
