QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/judgeMultiStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/judgeMultiStep
else:unix:!macx: TARGET = ../../../stepLib/judgeMultiStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += JUDGEMULTISTEP_LIBRARY

SOURCES += judgeMultiStep.cpp

HEADERS += judgeMultiStep.h\
    judgeMultiStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../../ -lTinyXml

LIBS += -L$$OUT_PWD/../../../ -lcondition

LIBS += -L$$OUT_PWD/../../../ -lCalcFormula

    
