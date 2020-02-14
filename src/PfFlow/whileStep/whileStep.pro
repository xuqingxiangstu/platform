QT       -= gui


win32:CONFIG(release, debug|release): TARGET = ../../../../stepLib/whileStep
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../stepLib/whileStep
else:unix:!macx: TARGET = ../../../stepLib/whileStep

TEMPLATE = lib
CONFIG += C++11
DEFINES += WHILESTEP_LIBRARY

SOURCES += whileStep.cpp

HEADERS += whileStep.h\
    whileStep_global.h

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



    
    
