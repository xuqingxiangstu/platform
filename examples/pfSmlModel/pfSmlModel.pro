#-------------------------------------------------
#
# Project created by QtCreator 2020-01-03T16:40:32
#
#-------------------------------------------------

QT       += core

QT       -= gui
win32:CONFIG(release, debug|release): TARGET = ../../../pfSmlModelDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../pfSmlModelDemo
else:unix:!macx: TARGET = ../../../pfSmlModelDemo

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
TEMPLATE = app


SOURCES += main.cpp \
    timer.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -lModelManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -lModelManager
else:unix:!macx:  LIBS += -L$$OUT_PWD/../../ -lModelManager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../algorithmLib/ -lrandaModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../algorithmLib/ -lrandaModel
else:unix:!macx: LIBS += -L$$OUT_PWD/../../algorithmLib/ -lrandaModel

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../algorithmLib/ -lwaveformModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../algorithmLib/ -lwaveformModel
else:unix:!macx:LIBS += -L$$OUT_PWD/../../algorithmLib/ -lwaveformModel

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../algorithmLib/ -ltemperatureModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../algorithmLib/ -ltemperatureModel
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../algorithmLib/ -ltemperatureModel

INCLUDEPATH += $$PWD/../../src/pfSmlModel
DEPENDPATH += $$PWD/../../src/pfSmlModel

HEADERS += \
    timer.h




