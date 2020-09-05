#include "argumentparse.h"

#include <sstream>
#include <QDebug>
#include <QTextCodec>

#include "middleargextract.h"
#include "feargextract.h"
#include "beargextract.h"

#include "../src/PfCommon/tools/ut_error.h"

argumentParse::argumentParse(QObject *parent) : QObject(parent)
{

    auto middleObj = std::make_shared<middleArgExtract>();

    connect(middleObj.get(), &middleArgExtract::writeToDb, this, &argumentParse::toDataBase);
    connect(middleObj.get(), &middleArgExtract::showResult, this, &argumentParse::toShowResult);
    connect(middleObj.get(), &middleArgExtract::showMessage, this, &argumentParse::showMessage);

    mArgExtracts[FRAME_MIDDLE] = middleObj;


    auto beObj = std::make_shared<beArgExtract>();

    connect(beObj.get(), &beArgExtract::writeToDb, this, &argumentParse::toDataBase);
    connect(beObj.get(), &beArgExtract::showResult, this, &argumentParse::toShowResult);
    connect(beObj.get(), &beArgExtract::showMessage, this, &argumentParse::showMessage);

    mArgExtracts[FRAME_BE] = beObj;


    auto feObj = std::make_shared<feArgExtract>();

    connect(feObj.get(), &feArgExtract::writeToDb, this, &argumentParse::toDataBase);
    connect(feObj.get(), &feArgExtract::showResult, this, &argumentParse::toShowResult);
    connect(feObj.get(), &feArgExtract::showMessage, this, &argumentParse::showMessage);

    mArgExtracts[FRAME_FE] = feObj;
}

void argumentParse::parse(QString uuid, Json::Value param, QByteArray vaildMsg)
{
    std::ostringstream errorInfo;

    mCurUuid = uuid;

    int frameType = param["frameType"].asInt();
    int rowIndex = param["rowIndex"].asInt();

    //step1:根据帧类型获取，解析类
    std::shared_ptr<PfIcdWorkBench::frameObj> frameObj = icdManager::getInstance()->getFrameObj(frameType);
    if(frameObj == nullptr)
    {
        errorInfo.str("");
        errorInfo << "[ERROR]" << "第" << std::dec << rowIndex << "行，获取帧类型 " << icdManager::getInstance()->getMapping(frameType).toStdString() << "失败";

        SHOW(errorInfo.str());

        emit showMessage(errorInfo.str().c_str(), false);

        return ;
    }

    Json::Value result;

    try
    {
        frameObj->parse((unsigned char*)vaildMsg.data(), vaildMsg.size(), result);                

        //参数提取
        QString frameKey = icdManager::getInstance()->getMapping(frameType);

        if(mArgExtracts.contains(frameKey))
        {
            mArgExtracts[frameKey]->extract(mCurUuid, param, frameObj, result);
        }
        else
        {
            SHOW("[ERROR] not find frame key -> " + frameKey.toStdString());
        }
    }    
    catch(std::runtime_error error)
    {
        SHOW(error.what());

        emit showMessage(error.what(), false);
    }
    catch(...)
    {
        emit showMessage("unkown error", false);
    }
}
