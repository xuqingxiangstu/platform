#include "projectcfg.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <sstream>

#include "../src/PfCommon/TinyXml/tinyxml.h"

projectCfg::projectCfg(QObject *parent)
    : QObject(parent),
    mDbPath(""),
    mPrjName(""),
    mPrjVersion(""),
    mDataFiles({}),
    mUuid(""),
    mPrjPath("")
{

}

QStringList projectCfg::copy(const QStringList &srcFiles)
{
    QStringList dstFiles;

    for(QString src : srcFiles)
    {
        QString dst = mPrjPath + "/data/" + src.right(src.size() - src.lastIndexOf("/") - 1);
        if(QFile::exists(src) && !QFile::exists(dst))
        {
            QFile::copy(src, dst);

            dstFiles.append(dst);
        }
    }

    return dstFiles;
}

void projectCfg::load(const QString &prjPath, const QString &uuid)
{
    std::ostringstream strErr;

    mPrjPath = prjPath;

    mUuid = uuid;

    mPrjFile = mPrjPath + "/" + prjPath.right(prjPath.size() - prjPath.lastIndexOf("/") - 1) + ".prj";

    mDataFiles.clear();

    TiXmlDocument *mXmlDoc = new TiXmlDocument(mPrjFile.toStdString().c_str());

    if(!mXmlDoc->LoadFile())
    {
        strErr << "[ERROR](xml)Open File Error" << mPrjFile.toStdString() << "\n";
        delete mXmlDoc;
        throw std::runtime_error(strErr.str());
    }

    TiXmlElement *projectEle = mXmlDoc->RootElement();

    if(projectEle)
    {
        const char *point = nullptr;

        point = projectEle->Attribute("name");
        if(point)
        {
            mPrjName = QString(point);
        }

        point = projectEle->Attribute("version");
        if(point)
        {
            mPrjVersion = QString(point);
        }

        point = projectEle->Attribute("uuid");
        if(point)
        {
            mUuid = QString(point);
        }

        TiXmlElement *dataEle = projectEle->FirstChildElement("data");
        if(dataEle)
        {
            TiXmlElement *mChildEle = dataEle->FirstChildElement("file");

            for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement())
            {                
                mDataFiles.append(mChildEle->GetText());
            }
        }

        TiXmlElement *resultEle = projectEle->FirstChildElement("result");
        if(resultEle)
        {
            mDbPath = resultEle->GetText();
        }
    }

    if(mDbPath.compare("") == 0)   //为空时说明以配置文件为主
    {
        mDbPath = mPrjPath + "/result/" + uuid + ".db";

    }

    delete mXmlDoc;
}

void projectCfg::save()
{
    TiXmlElement *projectEle = new TiXmlElement("project");

    projectEle->SetAttribute("name", mPrjName.toStdString().c_str());
    projectEle->SetAttribute("version", PRJ_VERSION);
    projectEle->SetAttribute("uuid", mUuid.toStdString().c_str());

    TiXmlElement *dataEle = new TiXmlElement("data");

    for(QString file : mDataFiles)
    {
        TiXmlElement *tixmlgEle = new TiXmlElement("file");

        TiXmlText *tixmlText = new TiXmlText(file.toStdString().c_str());

        tixmlgEle->LinkEndChild(tixmlText);

        dataEle->LinkEndChild(tixmlgEle);
    }

    projectEle->LinkEndChild(dataEle);

    TiXmlElement *tixmlgEle = new TiXmlElement("result");

    TiXmlText *tixmlText = new TiXmlText(mDbPath.toStdString().c_str());

    tixmlgEle->LinkEndChild(tixmlText);

    projectEle->LinkEndChild(tixmlgEle);


    //保存文件
    TiXmlDocument *myDocument = new TiXmlDocument();

    myDocument->LinkEndChild(projectEle);

    myDocument->SaveFile(mPrjFile.toStdString().c_str());

    delete myDocument;
    myDocument = nullptr;
}
