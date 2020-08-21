#-------------------------------------------------
#
# Project created by QtCreator 2020-08-11T10:12:09
#
#-------------------------------------------------

QT       -= gui


TARGET = rsaEncryption

win32{
    DESTDIR += ../../..
}

unix{
    DESTDIR += ../..
}

TEMPLATE = lib

DEFINES += RSAENCRYPTION_LIBRARY

SOURCES += rsaencryption.cpp

HEADERS += rsaencryption.h\
        rsaencryption_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32{
    LIBS += $$PWD/openssl/lib/MinGW/ssleay32.a
    LIBS += $$PWD/openssl/lib/MinGW/libeay32.a
}

unix{
    INCLUDEPATH += -I /usr/include/openssl
    LIBS += -L /usr/lib64 -lssl
    LIBS += -L /usr/lib64 -lcrypto
}
