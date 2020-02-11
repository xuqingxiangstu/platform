QT       -= gui

TARGET = ../../../../adapterLib/RelayCtrlAdapter
TEMPLATE = lib

DEFINES += RELAYCTRLADAPTER_LIBRARY
CONFIG += C++11
SOURCES += RelayCtrlAdapter.cpp

HEADERS += RelayCtrlAdapter.h\
      RelayCtrlAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lrelayctrl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lrelayctrl
