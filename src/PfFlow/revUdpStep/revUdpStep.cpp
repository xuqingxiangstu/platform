#include "revUdpStep.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
using namespace Pf;
revUdpStep::revUdpStep():
    mRunObj(nullptr),
    mAdapter(nullptr),
    strOutText(),
    mResult(false)
{
}

revUdpStep::~revUdpStep()
{

}

void revUdpStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;

    const char *point = nullptr;

    pTmpElem = xmlEle->FirstChildElement("AdapterId");
    if (pTmpElem)
    {
        point = pTmpElem->GetText();

        if(point)
            strDevId = std::string(point);
    }
}

std::string revUdpStep::getClassName()
{
    return VAR_NAME(revUdpStep);
}

bool revUdpStep::exec()
{
#ifdef DEBUG_OBJ
    UT_SHOW("[revUdpStep]");
#endif
    const int BUF_SIZE = 10240;
    char buf[BUF_SIZE] = {0};
    int rcvSize = 0;
    Pf::PfAdapter::Adapter *RcvObj;
    mRevResult = false;
    if(mAdapter->getAdapter(strDevId, &RcvObj))
    {
        //TODO:fjt
        if(RcvObj == nullptr)
            return false;
        if(RcvObj->receiveMsg(buf,rcvSize, BUF_SIZE))
        {
            std::shared_ptr<PfIcdWorkBench::icdFrameAdapter> frameAdapter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();
            try
            {
                //step2：初始化帧解析xml
                frameAdapter->init("./icd.xml");

               // qDebug() << "init Ok\n";

                //step3：获取待解析的帧句柄
                auto obj = frameAdapter->getFrameObj("1");
                if(obj != nullptr)
                {
//                    std::vector<PfIcdWorkBench::icdOutConvertValueType> outV;
                    //obj->parse(buf, rcvSize);
                    //obj->simulation(outV, msgJson);
                }
            }
            catch(std::runtime_error err)
            {

            }
            mRevResult = true;
        }
        else
        {
            //nothing to do
        }
    }
    else
    {
        UT_SHOW("get Adapter(" + strDevId + ") faild!");
        mResult = false;
    }
    return mResult;
}
std::string revUdpStep::getVarValue(std::string str)
{
    std::string res = "";

    if(str == "TableNumber")
    {
       res = std::to_string(usTableNumber);
    }
    else if(str=="CodeNumber")
    {
        res = std::to_string(usCodeNumber);
    }
    else if(str=="TableCodeNo")
    {
        unsigned int uiTemp=(usTableNumber<<16)|usCodeNumber;
        res = std::to_string(uiTemp);
    }else if(str=="Result")
    {
        res = std::to_string(mRevResult);
    }
    return res;
}

bool revUdpStep::getResult()
{
    return mResult;
}

std::string revUdpStep::outText()
{
    return strOutText;
}

void revUdpStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void revUdpStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new revUdpStep();
    }
}

    
    
