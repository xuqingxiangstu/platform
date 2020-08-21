#-------------------------------------------------
#
# Project created by QtCreator 2020-08-11T10:27:03
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = rsaEncryptionDemo

win32{
    DESTDIR += ../../..
}

unix{
    DESTDIR += ../..
}

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lrsaEncryption
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lrsaEncryption
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lrsaEncryption

