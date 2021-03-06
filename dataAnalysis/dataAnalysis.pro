#-------------------------------------------------
#
# Project created by QtCreator 2020-07-02T08:53:58
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += xml
QT       += charts
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:CONFIG(release, debug|release): TARGET =  ../../dataAnalysis
else:win32:CONFIG(debug, debug|release): TARGET =  ../../dataAnalysis
else:unix:!macx: TARGET =  ../dataAnalysisTool

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    projectnavigation.cpp \
    workspacearea.cpp \
    messagearea.cpp \
    fileanalysisbusiness.cpp \
    standardtime.cpp \
    datashowtable.cpp \
    newprjdialog.cpp \
    datashowwidget.cpp \
    dssingleimg.cpp \
    dsmuiltimg.cpp \
    versionform.cpp \
    dssingleupdatethread.cpp \
    dsfile.cpp \
    readfilethread.cpp \
    textedit.cpp \
    manualtestdialog.cpp

HEADERS  += mainwindow.h \
    projectnavigation.h \
    workspacearea.h \
    messagearea.h \
    fileanalysisbusiness.h \
    standardtime.h \
    datashowtable.h \
    newprjdialog.h \
    datashowwidget.h \
    dssingleimg.h \
    dsmuiltimg.h \
    versionform.h \
    dssingleupdatethread.h \
    dsfile.h \
    readfilethread.h \
    textedit.h \
    manualtestdialog.h

FORMS    += mainwindow.ui \
    projectnavigation.ui \
    workspacearea.ui \
    messagearea.ui \
    datashowtable.ui \
    newprjdialog.ui \
    datashowwidget.ui \
    dssingleimg.ui \
    dsmuiltimg.ui \
    versionform.ui \
    dsfile.ui \
    manualtestdialog.ui

include($$PWD/property/property.pri)
include($$PWD/propertyui/propertyui.pri)
include($$PWD/argParse/argParse.pri)
include($$PWD/fileAnalysis/fileAnalysis.pri)
include($$PWD/icdAdapter/icdAdapter.pri)
include($$PWD/argSave/argSave.pri)
include($$PWD/project/project.pri)
include($$PWD/chart/chart.pri)


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowserd
else:unix:!macx: LIBS += -L$$PWD/QtPropertyBrowser/lib/ -lQtPropertyBrowser

INCLUDEPATH += $$PWD/QtPropertyBrowser/include
DEPENDPATH += $$PWD/QtPropertyBrowser/include

LIBS += -L$$OUT_PWD/../ -lsqlTable

LIBS += -L$$OUT_PWD/../ -licdData

LIBS += -L$$OUT_PWD/../ -lcrc

LIBS += -L$$OUT_PWD/../ -ljsoncpp
LIBS += -L$$OUT_PWD/../ -lTinyXml

LIBS += -L$$OUT_PWD/../ -licdFrameAdapter

