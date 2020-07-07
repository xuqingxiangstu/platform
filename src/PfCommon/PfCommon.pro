TEMPLATE = subdirs

SUBDIRS +=  \
    TinyXml \
    recordLog \
    crc \
    jsoncpp \
    CalcFormula \
    cmdToJson \
    dynCreate

include (./tools/tools.pri)
