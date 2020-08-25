#-------------------------------------------------
#
# Project created by QtCreator 2020-03-21T09:25:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:CONFIG(release, debug|release): TARGET =  ../../eq
else:win32:CONFIG(debug, debug|release): TARGET =  ../../eq
else:unix:!macx: TARGET =  ../eqTool


CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    templatetree.cpp \
    flowtree.cpp \
    newdialog.cpp \
    recordnavigation.cpp \
    newprojectdialog.cpp \
    progresswidget.cpp \
    cmddecode.cpp \
    versionform.cpp \
    copypaste.cpp \
    triggercondition.cpp \
    searchform.cpp

HEADERS  += mainwindow.h \
    templatetree.h \
    flowtree.h \
    newdialog.h \
    recordnavigation.h \
    newprojectdialog.h \
    progresswidget.h \
    cmddecode.h \
    versionform.h \
    copypaste.h \
    triggercondition.h \
    searchform.h

FORMS    += mainwindow.ui \
    templatetree.ui \
    flowtree.ui \
    newdialog.ui \
    recordnavigation.ui \
    newprojectdialog.ui \
    progresswidget.ui \
    versionform.ui \
    searchform.ui

LIBS += -L$$OUT_PWD/../ -ljsoncpp

LIBS += -L$$OUT_PWD/../ -lsqlTable

include($$PWD/dragTree/dragTree.pri)
include($$PWD/property/property.pri)
include($$PWD/propertyui/propertyui.pri)
include($$PWD/toXml/toXml.pri)
include($$PWD/fromXml/fromXml.pri)

QT       += sql

INCLUDEPATH += $$PWD/dragTree
DEPENDPATH += $$PWD/dragTree

RESOURCES += \
    qrc.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowserd
else:unix:!macx: LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowser

INCLUDEPATH += $$PWD/QtPropertyBrowser/include
DEPENDPATH += $$PWD/QtPropertyBrowser/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowserd
else:unix:!macx: LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowser

INCLUDEPATH += $$PWD/QtPropertyBrowser/include
DEPENDPATH += $$PWD/QtPropertyBrowser/include

LIBS += -L$$OUT_PWD/../../../ -lTinyXml

include (./qtsingleapplication/qtsingleapplication.pri)
