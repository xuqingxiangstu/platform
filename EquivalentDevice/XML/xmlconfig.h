#ifndef XMLCONFIG_H
#define XMLCONFIG_H
#include "../../src/PfCommon/TinyXml/tinyxml.h"
class xmlConfig
{
public:
    xmlConfig(QString Name,QString CreateTime);
public:
    TiXmlElement *mRoot;
};

#endif // XMLCONFIG_H
