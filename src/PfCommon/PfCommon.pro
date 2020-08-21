TEMPLATE = subdirs

SUBDIRS +=  \
    TinyXml \
    jsoncpp \
    recordLog \
    crc \    
    CalcFormula \
    cmdToJson \
    dynCreate \
    rsaEncryption \
    qftp \
    encryption

include (./tools/tools.pri)
