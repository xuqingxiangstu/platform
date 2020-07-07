#include "decoding.h"

#include <QDebug>
#include "../virtualParams/virtualparams.h"
#include "../src/PfCommon/tools/ut_error.h"
#include "../flow/frame.h"
#include "../business/businessadapter.h"
#include "partpackage.h"

#include <QDateTime>
#include "../uibus.h"

decodingPool::decodingPool(QObject *parent):
    QObject(parent),
    mPfAdapterManager(nullptr)
{
//    mThreadPool = std::make_shared<QThreadPool>();
//    mThreadPool->setMaxThreadCount(10);  //最大线程数
//    mThreadPool->setExpiryTimeout(-1);
    mDecoding = std::make_shared<decoding>();
}

void decodingPool::decode(Json::Value param, QString recordUuid, QString uuid, QString ptl, QByteArray msg, QString rcvIp, int rcvPort)
{
    if(!mParseObj.contains(ptl))
        return ;

    Pf::PfAdapter::Adapter *adapterObj = nullptr;

    if(mPfAdapterManager.get())
    {
        mPfAdapterManager->getAdapter(uuid.toStdString(), &adapterObj);
    }

    mDecoding->exe_decoding(param, recordUuid.toStdString(), uuid.toStdString(), ptl.toStdString(), adapterObj, rcvIp.toStdString(), rcvPort, mParseObj[ptl], msg);

//    connect(obj, &decoding::result, this, &decodingPool::result);

//    mThreadPool->start(obj);
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

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> xkObj = mIcdWorkBench->getFrameObj(FRAME_XK);
    if(xkObj)
    {
        mParseObj[FRAME_XK] = xkObj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> dmObj = mIcdWorkBench->getFrameObj(FRAME_DM);
    if(dmObj)
    {
        mParseObj[FRAME_DM] = dmObj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> czxkObj = mIcdWorkBench->getFrameObj(FRAME_CZXK);
    if(czxkObj)
    {
        mParseObj[FRAME_CZXK] = czxkObj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> c3Obj = mIcdWorkBench->getFrameObj(FRAME_C3);
    if(c3Obj)
    {
        mParseObj[FRAME_C3] = c3Obj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> m1553BObj = mIcdWorkBench->getFrameObj(FRAME_M1553B);
    if(m1553BObj)
    {
        mParseObj[FRAME_M1553B] = m1553BObj;
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mJgobj = mIcdWorkBench->getFrameObj(FRAME_JG);
    if(mJgobj)
    {
        mParseObj[FRAME_JG] = mJgobj;
    }
}

/****************************************************/

decoding::decoding()
{
    mUiBus = uiBus::getInstance()->getUiAdapter();
}

void decoding::exe_decoding(Json::Value param, std::string recordUuid, std::string uuid, std::string ptl, Pf::PfAdapter::Adapter *adapterObj, const std::string &ipAddr, const int &port, std::shared_ptr<Pf::PfIcdWorkBench::frameObj> frameObj, QByteArray msg)
{
    mBusObj = adapterObj;
    mUuid = uuid;
    mCurPtrl = ptl;
    mFrameObj = frameObj;
    mCurMsg = msg;
    mDstIp = ipAddr;
    mDstPort = port;
    mRecordUuid = recordUuid;
    mParam = param;

    run();
}

decoding::~decoding()
{

}

void decoding::toUi(const std::string &msg, bool state)
{
    Json::Value js;
    js["msgType"] = "showMsg";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;

    if(state)
        tmp["status"] = "NORMAL";
    else
        tmp["status"] = "ERROR";

    tmp["show"] = "[" + QDateTime::currentDateTime().toString("hh:mm.ss.zzz").toStdString() + "]" + "[信息控制系统]->" + msg;

    js["msg"] = tmp;

    if(mUiBus)
        mUiBus->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());

}

void decoding::run()
{
    if( (mFrameObj.get() == nullptr) || (mCurMsg.size() <= 0) )
        return ;

    //解析
    try
    {        

        //modify xqx 20200422 TCP有粘包情况
        if("frameC3" == mFrameObj->getFrameName())
        {
            partPackage packages;
            packages.setHeadCode(QByteArray::fromHex("F7 7E F7 7E"));
            packages.setSrcPackage(mCurMsg);

            for(int index = 0; index < packages.getVaildPkSize(); index++)
            {
                QByteArray vaildMsg = packages.getVaildMsg(index);

                //step1:解析
                Json::Value result;
                mFrameObj->parse((unsigned char*)vaildMsg.data(), vaildMsg.size(), result);

                //step2：业务处理

                Pf::PfIcdWorkBench::byteArray readMsg;
                std::copy((unsigned char*)vaildMsg.data(), (unsigned char*)vaildMsg.data() + vaildMsg.size(), std::back_inserter(readMsg));

                auto businessObj = businessAdapter::getInstance()->getBusiness(mCurPtrl);
                businessObj->setBusObj(mBusObj, mDstIp, mDstPort);
                businessObj->setFrameObj(mFrameObj);
                businessObj->setDevUuid(mUuid);
                businessObj->setRecordUuid(mRecordUuid);
                if(businessObj)
                {
                    businessObj->deal(readMsg, result);
                }
            }
        }
        else if("frameXK" == mFrameObj->getFrameName())
        {
            partPackage packages;
            packages.setHeadCode(QByteArray::fromHex("EB 90"));
            packages.setSrcPackage(mCurMsg);

            for(int index = 0; index < packages.getVaildPkSize(); index++)
            {
                QByteArray vaildMsg = packages.getVaildMsg(index);

                //step1:解析
                Json::Value result;
                mFrameObj->parse((unsigned char*)vaildMsg.data(), vaildMsg.size(), result);

                //step2：业务处理

                Pf::PfIcdWorkBench::byteArray readMsg;
                std::copy((unsigned char*)vaildMsg.data(), (unsigned char*)vaildMsg.data() + vaildMsg.size(), std::back_inserter(readMsg));

                auto businessObj = businessAdapter::getInstance()->getBusiness(mCurPtrl);
                if(businessObj)
                {
                    businessObj->setBusObj(mBusObj, mDstIp, mDstPort);
                    businessObj->setFrameObj(mFrameObj);
                    businessObj->setDevUuid(mUuid);
                    businessObj->setRecordUuid(mRecordUuid);
                    businessObj->setParam(mParam);

                    businessObj->deal(readMsg, result);
                }
            }
        }
        else
        {
            //step1:解析
            Json::Value result;
            mFrameObj->parse((unsigned char*)mCurMsg.data(), mCurMsg.size(), result);

            //step2：业务处理

            Pf::PfIcdWorkBench::byteArray readMsg;
            std::copy((unsigned char*)mCurMsg.data(), (unsigned char*)mCurMsg.data() + mCurMsg.size(), std::back_inserter(readMsg));

            auto businessObj = businessAdapter::getInstance()->getBusiness(mCurPtrl);
            if(businessObj)
            {
                businessObj->setBusObj(mBusObj, mDstIp, mDstPort);
                businessObj->setFrameObj(mFrameObj);
                businessObj->setDevUuid(mUuid);
                businessObj->setRecordUuid(mRecordUuid);
                businessObj->setParam(mParam);

                businessObj->deal(readMsg, result);
            }
        }
    }
    catch(std::runtime_error err)
    {
        toUi(err.what(), false);
        UT_SHOW(err.what());
    }
    catch(Json::LogicError err)
    {
        toUi(err.what(), false);
        UT_SHOW(err.what());
    }
    catch(...)
    {
        toUi("[ERROR]...", false);
        UT_SHOW("[ERROR]...");
    }
}
