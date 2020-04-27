QT       -= gui

QT += network

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/tcpAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/tcpAdapter
else:unix:!macx: TARGET = ../../../adapterLib/tcpAdapter
TEMPLATE = lib

DEFINES += TCPADAPTER_LIBRARY
CONFIG += c++11
SOURCES += tcpAdapter.cpp

HEADERS += tcpAdapter.h\
      tcpAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
    
LIBS += -L$$OUT_PWD/../../../ -ljsoncpp
LIBS += -L$$OUT_PWD/../../../ -ltcpClient
