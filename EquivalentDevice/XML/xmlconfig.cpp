#include "xmlconfig.h"
#include <QDir>
#include <QMessageBox>
#include <QObject>
xmlConfig::xmlConfig(QString Name,QString CreateTime)
{
    //QString qstrFileName = QDir::currentPath()+"/XmlCfgFile/"+Name+"_"+ CreateTime+".xml";
    QString qstrFileName = QDir::currentPath()+"/XmlCfgFile/Sleep-if-elseif-end.xml";
    std::string strFilePath=qstrFileName.toStdString();
    TiXmlDocument mXmlDoc(strFilePath.c_str());
    if(!mXmlDoc.LoadFile())
    {
        QMessageBox::warning(0, QObject::tr("xml Error"),qstrFileName);
    }
    else
    {
        mRoot = mXmlDoc.RootElement();
    }
   std::string str = mRoot->Value();
   str = mRoot->Value();
}

