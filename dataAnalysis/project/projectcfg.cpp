#include "projectcfg.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <sstream>

#include "../src/PfCommon/TinyXml/tinyxml.h"

projectCfg::projectCfg(QObject *parent) : QObject(parent)
{

}


void projectCfg::loadCfg(const QString &mCurPath)
{
    std::ostringstream strErr;
    TiXmlDocument *mXmlDoc = new TiXmlDocument(mCurPath.toStdString().c_str());

    if(!mXmlDoc->LoadFile())
    {
        strErr << "[ERROR](xml)Open File Error" << mCurPath.toStdString() << "\n";
        delete mXmlDoc;
        throw std::runtime_error(strErr.str());
    }

    TiXmlElement *projectEle = mXmlDoc->RootElement();

    //TiXmlElement *projectEle = mRoot->FirstChildElement("project");
    if(projectEle)
    {
        TiXmlElement *dataEle = projectEle->FirstChildElement("data");
        if(dataEle)
        {
            TiXmlElement *mChildEle = dataEle->FirstChildElement("file");

            for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement())
            {
                qDebug() << mChildEle->GetText();
            }
        }

        TiXmlElement *resultEle = projectEle->FirstChildElement("result");
        if(resultEle)
        {
            qDebug() << resultEle->GetText();
        }
    }

    delete mXmlDoc;
}
