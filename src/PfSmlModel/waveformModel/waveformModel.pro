QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../algorithmLib/waveformModel
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../algorithmLib/waveformModel
else:unix:!macx: TARGET = ../../../algorithmLib/waveformModel

TEMPLATE = lib

DEFINES += WAVEFORMMODEL_LIBRARY
CONFIG += C++11
SOURCES += waveformModel.cpp

HEADERS += waveformModel.h\
      waveformModel_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
    
