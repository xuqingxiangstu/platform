QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/formulaCalcStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/formulaCalcStep
else:unix:!macx: TARGET = ../../../stepLib/formulaCalcStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += FORMULACALCSTEP_LIBRARY

SOURCES += formulaCalcStep.cpp

HEADERS += formulaCalcStep.h\
    formulaCalcStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml


    
    
