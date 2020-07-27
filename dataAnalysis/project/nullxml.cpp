#include "nullxml.h"

#include "../src/PfCommon/TinyXml/tinyxml.h"

void nullXml::create(const std::string &filePath)
{
    TiXmlDocument myDocument;

    myDocument.LinkEndChild(new TiXmlElement("project"));

    bool res = myDocument.SaveFile(filePath.c_str());
    int i = 0;
    i++;
}

void nullXml::create(QString filePath)
{
    TiXmlDocument myDocument;

    myDocument.LinkEndChild(new TiXmlElement("project"));

    bool res = myDocument.SaveFile(filePath.toLocal8Bit().data());
}
