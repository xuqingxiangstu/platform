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
    mArgExtracts[FRAME_MIDDLE] = std::make_shared<middleArgExtract>();
    mArgExtracts[FRAME_BE] = std::make_shared<beArgExtract>();
    mArgExtracts[FRAME_FE] = std::make_shared<feArgExtract>();
}

void argumentParse::parse(Json::Value param, QByteArray vaildMsg)
{
    std::ostringstream errorInfo;

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
            mArgExtracts[frameKey]->extract(param, frameObj, result);
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
}
