QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/judgeStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/judgeStep
else:unix:!macx: TARGET = ../../../stepLib/judgeStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += JUDGESTEP_LIBRARY

SOURCES += judgeStep.cpp

HEADERS += judgeStep.h\
    judgeStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lTinyXml

LIBS += -L$$OUT_PWD/../../../ -lcondition

LIBS += -L$$OUT_PWD/../../../ -lCalcFormula

    
    
