#-------------------------------------------------
#
# Project created by QtCreator 2020-02-14T16:21:57
#
#-------------------------------------------------

QT       += core
QT += network
QT       -= gui
CONFIG += c++14

win32:CONFIG(release, debug|release): TARGET =  ../../logicLayer
else:win32:CONFIG(debug, debug|release): TARGET =  ../../logicLayer
else:unix:!macx: TARGET =  ../logicLayerExe



CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#DEFINES += QT_NO_WARNING_OUTPUT # 屏蔽警告输出
#DEFINES += QT_NO_DEBUG_OUTPUT   # 屏蔽qDebug输出
#DEFINES += QT_NO_INFO_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    rcvcmdtask.cpp \
    cmddecode.cpp \
    uibus.cpp

LIBS += -L$$OUT_PWD/../ -lrecordLog

#LIBS += -L$$OUT_PWD/../stepLib/ -lRunFlow

LIBS += -L$$OUT_PWD/../ -lPfAdapterManager

LIBS += -L$$OUT_PWD/../adapterLib/ -lZmqRcvAdapter
LIBS += -L$$OUT_PWD/../adapterLib/ -lZmqSendAdapter
LIBS += -L$$OUT_PWD/../ -lzmq

LIBS += -L$$OUT_PWD/../adapterLib/ -lUnicastAdapter
LIBS += -L$$OUT_PWD/../ -ludp

LIBS += -L$$OUT_PWD/../adapterLib/ -ltcpAdapter
LIBS += -L$$OUT_PWD/../ -ltcpClient

LIBS += -L$$OUT_PWD/../adapterLib/ -ltcpServerAdapter
LIBS += -L$$OUT_PWD/../ -ltcpServer

LIBS += -L$$OUT_PWD/../adapterLib/ -lm1553Adapter
LIBS += -L$$OUT_PWD/../ -lM1553B

LIBS += -L$$OUT_PWD/../ -ljsoncpp

LIBS += -L$$OUT_PWD/../ -lcmdToJson

LIBS += -L$$OUT_PWD/../ -lzeroMq

LIBS += -L$$OUT_PWD/../ -licdFrameAdapter

LIBS += -L$$OUT_PWD/../ -lTinyXml

LIBS += -L$$OUT_PWD/../ -ldynCreate

LIBS += -L$$OUT_PWD/../ -lmodelManager

LIBS += -L$$OUT_PWD/../algorithmLib/ -lrandaModel

LIBS += -L$$OUT_PWD/../algorithmLib/ -ltemperatureModel

LIBS += -L$$OUT_PWD/../ -lsqlTable

LIBS += -L$$OUT_PWD/../../../ -lcrc

LIBS += -L$$OUT_PWD/../../../ -licdData

HEADERS += \
    rcvcmdtask.h \
    cmddecode.h \
    uibus.h

include (./flow/flow.pri)
include (./task/task.pri)
include (./decoding/decoding.pri)
include (./singleTest/singleTest.pri)
include (./custom/custom.pri)
include (./business/business.pri)
include (./logRecord/logRecord.pri)
include (./virtualParams/virtualParams.pri)
