#-------------------------------------------------
#
# Project created by QtCreator 2020-08-11T15:03:33
#
#-------------------------------------------------

QT       -= gui
QT += network
TARGET = qftp

win32{
    DESTDIR += ../../..
}

unix{
    DESTDIR += ../..
}

TEMPLATE = lib

DEFINES += QFTP_LIBRARY

SOURCES += qftp.cpp \
    qurlinfo.cpp

HEADERS += qftp.h\
        qftp_global.h \
    qurlinfo.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
