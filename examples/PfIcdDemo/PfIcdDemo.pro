#-------------------------------------------------
#
# Project created by QtCreator 2020-02-10T15:41:41
#
#-------------------------------------------------

QT       += core
QT       += xlsx
QT       -= gui

CONFIG += C++11

win32:CONFIG(release, debug|release): TARGET = ../../../PfIcdDemo
else:win32:CONFIG(debug, debug|release): TARGET = ../../../PfIcdDemo
else:unix:!macx: TARGET = ../../PfIcdDemo

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ -licdFrameAdapter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ -licdFrameAdapter
else:unix:!macx: LIBS += -L$$OUT_PWD/../../ -licdFrameAdapter

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lcrc
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lcrc
