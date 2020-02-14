QT       -= gui


win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/elseIfStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/elseIfStep
else:unix:!macx: TARGET = ../../../stepLib/elseIfStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += ELSEIFSTEP_LIBRARY

SOURCES += elseIfStep.cpp

HEADERS += elseIfStep.h\
    elseIfStep_global.h

unix {
target.path = /usr/lib
INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcondition
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcondition
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lcondition

    
