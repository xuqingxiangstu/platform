#include "nullxml.h"

#include "createsubflow.h"

void nullXml::create(std::string filePath)
{
    TiXmlDocument myDocument;

    myDocument.LinkEndChild(createRoot::element());

    myDocument.SaveFile(filePath.c_str());
}
