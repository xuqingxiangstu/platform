#include "nullxml.h"

#include "../src/PfCommon/TinyXml/tinyxml.h"

void nullXml::create(const std::string &filePath)
{
    TiXmlDocument myDocument;

    myDocument.LinkEndChild(new TiXmlElement("project"));

    myDocument.SaveFile(filePath.c_str());
}

void nullXml::create(const QString &filePath)
{
    create(filePath.toStdString());
}
