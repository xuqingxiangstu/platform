TEMPLATE = subdirs

SUBDIRS +=  \
    TinyXml \
    jsoncpp \
    recordLog \
    crc \    
    CalcFormula \
    cmdToJson \
    dynCreate \    
    qftp \
    encryption

include (./tools/tools.pri)
