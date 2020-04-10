QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../algorithmLib/temperatureModel
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../algorithmLib/temperatureModel
else:unix:!macx: TARGET = ../../../algorithmLib/temperatureModel

TEMPLATE = lib

DEFINES += TEMPERATUREMODEL_LIBRARY
CONFIG += C++11
SOURCES += temperatureModel.cpp

HEADERS += temperatureModel.h\
      temperatureModel_global.h

unix {
  target.path = /usr/lib
  INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml
    
    
