#-------------------------------------------------
#
# Project created by QtCreator 2020-08-21T15:13:01
#
#-------------------------------------------------

QT       -= gui

TARGET = encryption
TEMPLATE = lib

win32{
    DESTDIR += ../../..
}

unix{
    DESTDIR += ../..
}

DEFINES += ENCRYPTION_LIBRARY

SOURCES += \
    rsa.cpp \
    rc4.cpp

HEADERS +=\
        encryption_global.h \
    rsa.h \
    rc4.h

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
