QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../adapterLib/ZmqSendAdapter
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../adapterLib/ZmqSendAdapter
else:unix:!macx: TARGET = ../../../adapterLib/ZmqSendAdapter
TEMPLATE = lib

DEFINES += ZMQSENDADAPTER_LIBRARY
CONFIG += c++11
SOURCES += ZmqSendAdapter.cpp

HEADERS += ZmqSendAdapter.h\
      ZmqSendAdapter_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lzeroMq
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lzeroMq
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lzeroMq

    
