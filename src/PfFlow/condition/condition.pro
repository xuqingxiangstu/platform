#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T14:41:51
#
#-------------------------------------------------

QT       -= gui

win32:CONFIG(release, debug|release): TARGET = ../../../../condition
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../condition
else:unix:!macx: TARGET = ../../../condition

TEMPLATE = lib

DEFINES += CONDITION_LIBRARY
CONFIG += C++11
SOURCES += condition.cpp \
    data.cpp

HEADERS += condition.h\
        condition_global.h \
    data.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml

LIBS += -L$$OUT_PWD/../../../ -lCalcFormula
