#-------------------------------------------------
#
# Project created by QtCreator 2020-03-09T10:10:25
#
#-------------------------------------------------

QT       += core gui
QT       += sql xml
QT       += xlsx
CONFIG += c++11

win32:CONFIG(release, debug|release): TARGET =  ../../EquivalentDevice
else:win32:CONFIG(debug, debug|release): TARGET =  ../../EquivalentDevice
else:unix:!macx: TARGET =  ../EquivalentDeviceExe

LIBS += -L$$OUT_PWD/../ -ljsoncpp
LIBS += -L$$OUT_PWD/../ -lTinyXml

LIBS += -L$$OUT_PWD/../ -lPfAdapterManager

LIBS += -L$$OUT_PWD/../ -lcmdToJson


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

RC_ICONS = icon128.ico


SOURCES += main.cpp\
        mainwindow.cpp \
    sysManage/addsysmanage.cpp \
    sysManage/sysmanageui.cpp \
    flow/flow.cpp \
    flow/dataselect.cpp \
    simulation/simulation.cpp \
    dbtableopt.cpp \
    selectmenufrm.cpp \
    XML/xmlconfig.cpp \
    delegate/combodelegate.cpp \
    delegate/datedelegate.cpp \
    delegate/spindelegate.cpp \
    test/cmddecode.cpp \
    test/testflow.cpp \
    excelToDb/excelToDb.cpp \
    dbToExcel/dbToExcel.cpp \
    dbToExcel/exportcmd.cpp \
    login.cpp \
    delegate/qtreeitemdelegate.cpp \
    excelToDb/importcmd.cpp

HEADERS  += mainwindow.h \
    sysManage/addsysmanage.h \
    sysManage/sysmanageui.h \
    flow/flow.h \
    flow/dataselect.h \
    simulation/simulation.h \
    dbtableopt.h \
    selectmenufrm.h \
    XML/xmlconfig.h \
    delegate/combodelegate.h \
    delegate/datedelegate.h \
    delegate/spindelegate.h \
    test/cmddecode.h \
    test/testflow.h \
    excelToDb/excelToDb.h \
    dbToExcel/dbToExcel.h \
    dbToExcel/exportcmd.h \
    login.h \
    delegate/qtreeitemdelegate.h \
    excelToDb/importcmd.h

FORMS    += mainwindow.ui \
    sysManage/addsysmanage.ui \
    sysManage/sysmanageui.ui \
    flow/flow.ui \
    flow/dataselect.ui \
    simulation/simulation.ui \
    selectmenufrm.ui \
    test/testflow.ui \
    dbToExcel/exportcmd.ui \
    login.ui \
    delegate/qtreeitemdelegate.ui \
    excelToDb/importcmd.ui

RESOURCES += \
    icon.qrc \
    image.qrc

