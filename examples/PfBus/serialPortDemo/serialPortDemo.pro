#-------------------------------------------------
#
# Project created by QtCreator 2020-01-03T10:48:49
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = serialPortDemo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lniSerialPort
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lniSerialPort

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lserialPort
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lserialPort

INCLUDEPATH += $$PWD/../../../src/PfBus/serialPort
DEPENDPATH += $$PWD/../../../src/PfBus/serialPort
