#include "decoding.h"

#include <QDebug>
#include "../virtualParams/virtualparams.h"
#include "../src/PfCommon/tools/ut_error.h"
#include "../flow/frame.h"

decodingPool::decodingPool(QObject *parent):
    QObject(parent),
    mPfAdapterManager(nullptr)
{
    mThreadPool = std::make_shared<QThreadPool>();
    mThreadPool->setMaxThreadCount(4);  //最大线程数
    mThreadPool->setExpiryTimeout(-1);
}

void decodingPool::decode(QString uuid, QString ptl, QByteArray msg, QString rcvIp, int rcvPort)
{
    if(!mParseObj.contains(ptl))
        return ;

    Pf::PfAdapter::Adapter *adapterObj = nullptr;

    if(mPfAdapterManager.get())
    {
        mPfAdapterManager->getAdapter(uuid.toStdString(), &adapterObj);
    }

    decoding *obj = new decoding(uuid.toStdString(), adapterObj, rcvIp.toStdString(), rcvPort, mParseObj[ptl], msg);

    connect(obj, &decoding::result, this, &decodingPool::result);

    mThreadPool->start(obj);
}

void decodingPool::setIcdFrameAdpter(std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> obj)
{
    mIcdWorkBench = obj;

    //初始化各解析器（BE、FE、93、中间件）

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> beObj = mIcdWorkBench->getFrameObj(FRAME_BE);
    if(beObj)
    {
        mParseObj[FRAME_BE] = beObj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> feObj = mIcdWorkBench->getFrameObj(FRAME_FE);
    if(feObj)
    {
        mParseObj[FRAME_FE] = feObj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> f93Obj = mIcdWorkBench->getFrameObj(FRAME_93);
    if(f93Obj)
    {
        mParseObj[FRAME_93] = f93Obj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> middleObj = mIcdWorkBench->getFrameObj(FRAME_MIDDLE);
    if(middleObj)
    {
        mParseObj[FRAME_MIDDLE] = middleObj;
    }
}

/****************************************************/

decoding::decoding(std::string uuid, Pf::PfAdapter::Adapter *adapterObj, const std::string &ipAddr, const int &port, std::shared_ptr<Pf::PfIcdWorkBench::frameObj> frameObj, QByteArray msg):
    mBusObj(adapterObj),
    mUuid(uuid),
    mFrameObj(frameObj),
    mCurMsg(msg),
    mDstIp(ipAddr),
    mDstPort(port)
{

}

decoding::~decoding()
{

}

void decoding::run()
{
    if( (mFrameObj.get() == nullptr) || (mCurMsg.size() <= 0) )
        return ;

    //解析
    try
    {
        //step1:解析
        Json::Value result;
        mFrameObj->parse((unsigned char*)mCurMsg.data(), mCurMsg.size(), result);

        //step2：是否应答
        Pf::PfIcdWorkBench::byteArray readMsg;
        std::copy((unsigned char*)mCurMsg.data(), (unsigned char*)mCurMsg.data() + mCurMsg.size(), std::back_inserter(readMsg));

        Pf::PfIcdWorkBench::byteArray askMsg;
        if(mFrameObj->getAskMsg(readMsg, askMsg, result))
        {
            mBusObj->sendMsg((const char*)&askMsg.at(0), askMsg.size(), mDstIp, mDstPort);
        }

        //step3：更新参数
        Json::Value newJs;
        if(mFrameObj->getValidValue(result, newJs))
        {
            for(int index = 0; index < newJs.size(); index++)
            {
                int coding = 0, table = 0;
                if(!newJs[index]["coding"].isNull())
                {
                    coding = newJs[index]["coding"].asInt();
                }

                if(!newJs[index]["table"].isNull())
                {
                    table = newJs[index]["table"].asInt();
                }

                virtualParams::getInstance()->setValue({mUuid, std::to_string(table), std::to_string(coding)}, mapValue());
            }
        }

        UT_SHOW(result.toStyledString());
    }
    catch(std::runtime_error err)
    {
        UT_SHOW(err.what());
    }
    catch(Json::LogicError err)
    {
        UT_SHOW(err.what());
    }
}
