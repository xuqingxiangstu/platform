#include <QCoreApplication>

#include <QDebug>

#include "../../../src/PfCommon/TinyXml/tinyxml.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "tinyxml test";

    TiXmlDocument mXmlDoc ("");

    mXmlDoc.LoadFile();

    return a.exec();
}
