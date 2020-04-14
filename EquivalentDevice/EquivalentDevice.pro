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
else:unix:!macx: TARGET =  ../EquivalentDevice

LIBS += -L$$OUT_PWD/../ -ljsoncpp
LIBS += -LSSOUT_PWD/../ -lTinyXml

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lPfAdapterManager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lPfAdapterManager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ -lcmdToJson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ -lcmdToJson


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app


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
    excelToDb/importcmd.cpp \
    dbToExcel/dbToExcel.cpp \
    dbToExcel/exportcmd.cpp \
    login.cpp \
    delegate/qtreeitemdelegate.cpp

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
    excelToDb/importcmd.h \
    dbToExcel/dbToExcel.h \
    dbToExcel/exportcmd.h \
    login.h \
    delegate/qtreeitemdelegate.h

FORMS    += mainwindow.ui \
    sysManage/addsysmanage.ui \
    sysManage/sysmanageui.ui \
    flow/flow.ui \
    flow/dataselect.ui \
    simulation/simulation.ui \
    selectmenufrm.ui \
    test/testflow.ui \
    excelToDb/importcmd.ui \
    dbToExcel/exportcmd.ui \
    login.ui

RESOURCES += \
    icon.qrc \
    image.qrc


