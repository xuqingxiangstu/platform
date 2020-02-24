#include "autorun.h"

#include "runprocedure.h"
#include <thread>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "../../PfCommon/tools/ut_error.h"
#include "../../PfCommon/cmdToJson/cmdtojson.h"
using namespace Pf;
autoRun::autoRun()
    :mIsExit(false),
      mIsRunning(false),
      mMsgObj(nullptr),
      mCurItemIndex(0)
{

}

autoRun::~autoRun()
{
    unInit();
}

void autoRun::updateMsg(const std::string &msg)
{
    ///modify xqx 20191113 增加 "flow": 类型

   /* std::string tmpMsg;

    ///找到"type"关键字
    std::string::size_type typeIndex = msg.find("type");
    if(typeIndex != std::string::npos)
    {
        ///找到 type之后的 ','
        std::string::size_type fliteIndex = msg.substr(typeIndex, msg.size() - typeIndex).find_first_of(',');

        std::string frist = msg.substr(0, typeIndex + fliteIndex);
        std::string second = msg.substr(typeIndex + fliteIndex, msg.size() - typeIndex - fliteIndex);

        ///增加 "flow"字段

        tmpMsg += frist;
        tmpMsg += ",\"flow\":\"";
        tmpMsg += mCurFlowType;
        tmpMsg += "\"";
        tmpMsg += second;
    }
    else
    {
        tmpMsg = msg;
    }
*/
    ///end

    if(mMsgObj)
        mMsgObj->sendMsg(msg.c_str(), msg.size());
}

void autoRun::runAllFlow()
{
    if(mIsRunning)
        return ;

    std::thread exeThread([&]()
    {
        mIsRunning = true;
        std::vector<std::string> VecCode;
        //basic::bl_single_instance<conditionManager>::getInstance()->resetCondition();

        try
        {
            //执行项目
            for (auto itor = mRunVec.begin(); itor != mRunVec.end(); ++itor)
            {
                std::string des = (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->getDescribe();
                std::string strFormat = "-----------";

                VecCode.clear();
                VecCode.push_back("showMsg");
                VecCode.push_back("normal");
                VecCode.push_back(strFormat + des + strFormat);
                std::string strCode=PfCommon::cmdToJson::replyCtrlCmd(VecCode,true);
                updateMsg(strCode);

                //设置父节点
                (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->setFatherId(std::get<KeyIndex>(*itor));
                //进行测试
                (std::get<ObjIndex>(*itor))->exec();

                bool testRes = (std::get<ObjIndex>(*itor))->getResult();
#if 0
                ///错误记录，ＥＸＣＥＬ　modify xqx 20191015
                VecCode.clear();

                /*if(testRes)
                    strCode << "{\"msgType\":\"runResult\",\"param\":{\"res\":\"true\",\"cell\":\"" << std::get<1>(*itor) << "\"}}";
                else
                    strCode << "{\"msgType\":\"runResult\",\"param\":{\"res\":\"false\",\"cell\":\"" << std::get<1>(*itor) << "\"}}";

                updateMsg(strCode.str());*/
#endif
                //退出测试
                if(mIsExit)
                    break;
            }
        }
        catch(std::runtime_error err)
        {

        }

        //生产错误报告

        bool allTestRes = true;

        for (auto itor = mRunVec.begin(); itor != mRunVec.end(); ++itor)
        {
            allTestRes &= (std::get<ObjIndex>(*itor))->getResult();
        }

        //结束后向界面发送结束消息     
        std::vector<std::string> vecMsg;
        vecMsg.clear();
        vecMsg.push_back("over");
        vecMsg.push_back("normal");
        if(allTestRes)
        {
            std::string strMsg=PfCommon::cmdToJson::replyCtrlCmd(vecMsg,true);
            updateMsg(strMsg);
        }
        else
        {
            std::string strMsg=PfCommon::cmdToJson::replyCtrlCmd(vecMsg,false);
            updateMsg(strMsg);
        }


        mIsExit = false;

        mIsRunning = false;
    }
    );

    exeThread.detach();
}

//退出所有测试

void autoRun::exitAllTest()
{
    if(!mIsRunning)
        return ;

    mIsExit = true;

    //设置退出标志

    for (auto itor = mRunVec.begin(); itor != mRunVec.end(); ++itor)
    {
        (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->exitTest();
    }

    mIsRunning = false;
}

//暂停测试
void autoRun::suspendTest()
{
    for (auto itor = mRunVec.begin(); itor != mRunVec.end(); ++itor)
    {
        (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->suspendTest();
    }
}
//继续测试
void autoRun::continueTest()
{
    for (auto itor = mRunVec.begin(); itor != mRunVec.end(); ++itor)
    {
        (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->continueTest();
    }
}

void autoRun::runFlow(const std::string &id)
{
    if(mIsRunning)
        return ;

    auto ifFind = std::find_if(mRunVec.begin(), mRunVec.end(), [=](const ValueType & v)
                    {
                        return std::get<KeyIndex>(v) == id;
                    }
                 );
    if(ifFind == mRunVec.end())
    {
        UT_THROW_EXCEPTION("not fond item(" + id + ")");
    }

    mCurItemIndex = ifFind - mRunVec.begin();

    std::thread exeThread([&]()
    {
        mIsRunning = true;
        bool allTestRes = true;
        std::vector<std::string> vecMsg;

        try
        {
            auto itor = mRunVec.begin() + mCurItemIndex;

            std::string des = (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->getDescribe();
            std::string strFormat = "-----------";
            vecMsg.clear();
            vecMsg.push_back("showMsg");
            vecMsg.push_back("normal");
            vecMsg.push_back(strFormat + des + strFormat);
            std::string strMsg=PfCommon::cmdToJson::replyCtrlCmd(vecMsg,true);
            updateMsg(strMsg);
            //设置父节点
            (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->setFatherId(std::get<KeyIndex>(*itor));
            //进行测试
            (std::get<ObjIndex>(*itor))->exec();

            allTestRes = (std::get<ObjIndex>(*itor))->getResult();
        }
        catch(std::runtime_error err)
        {

        }
        //结束后向界面发送结束消息
        vecMsg.clear();
        vecMsg.push_back("over");
        vecMsg.push_back("normal");
        if(allTestRes)
        {
            std::string strMsg=PfCommon::cmdToJson::replyCtrlCmd(vecMsg,true);
            updateMsg(strMsg);
        }
        else
        {
            std::string strMsg=PfCommon::cmdToJson::replyCtrlCmd(vecMsg,false);
            updateMsg(strMsg);
        }

        mIsExit = false;
        mIsRunning = false;
    }
    );

    exeThread.detach();
}



void autoRun::getRunItems(std::vector<std::string> &items)
{
    std::vector<ValueType>::iterator itor = mRunVec.begin();

    for (; itor != mRunVec.end(); ++itor)
    {
        std::ostringstream str;
        TestProcedure *pro = dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor));

        str << std::get<KeyIndex>(*itor)<< ":" << pro->getDescribe();


        std::vector<std::string> strItems;
        pro->getRunItems(strItems);

        if (strItems.size() > 0)
        {
            str << "{";
        }

        for (unsigned int i = 0; i < strItems.size(); i++)
        {
            str << strItems.at(i) << ":";
        }

        str << "}";

        items.push_back(str.str());
    }
}


runObj *autoRun::getObj(std::string proceIndex, std::string testCaseIndex, std::string stepIndex)
{
    auto isFind = std::find_if(mRunVec.begin(), mRunVec.end(), [=](const ValueType &v){
       return  std::get<KeyIndex>(v) == proceIndex;
    });

    if(isFind != mRunVec.end())
    {
        return dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*isFind))->getObj(testCaseIndex, stepIndex);
    }

    return nullptr;
}

void autoRun::setFlowType(std::string type)
{
    mCurFlowType = type;

    for (auto itor = mRunVec.begin(); itor != mRunVec.end(); ++itor)
    {
        (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->setFlowType(type);
    }
}

void autoRun::setMsgObj(Pf::PfAdapter::Adapter *obj)
{
    mMsgObj = obj;

    for (auto itor = mRunVec.begin(); itor != mRunVec.end(); ++itor)
    {
        (dynamic_cast<TestProcedure*>(std::get<ObjIndex>(*itor)))->setMsgObj(obj);
    }
}

void autoRun::init(TiXmlElement *xmlEle)
{
    std::ostringstream strErr;
    TiXmlElement *mChildEle = xmlEle->FirstChildElement();

    std::string str = mChildEle->Value();
    if (strcmp(str.c_str(), ProcedureNode))
    {
        strErr.str("");
        strErr << "[ERROR]CFlow Init not is (TestProcedure) is(" << str << ")";
        UT_THROW_EXCEPTION(strErr.str());
    }

    for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement())
    {
        if (mChildEle)
        {
            const char *point = mChildEle->Attribute(KeyAttr);
            std::string strId = "";
            if(nullptr == point)
            {
                UT_THROW_EXCEPTION("get Index Attribute faild!");
            }

            strId = std::string(point);

            point = mChildEle->Attribute(NodeEnableAttr);
            if(point)
            {
                if(std::string(point) == "false")
                    continue;
            }

            std::vector<ValueType>::iterator itor;

            itor = std::find_if(mRunVec.begin(), mRunVec.end(), [=](const ValueType &v){
                return std::get<KeyIndex>(v) == strId;
            });

            if (itor != mRunVec.end())
            {
                strErr.str("");
                strErr << "[ERROR]CFlow" << "测试ID(" << strId << ")" << "重复定义";
                UT_THROW_EXCEPTION(strErr.str());
            }

            runObj *obj = new TestProcedure();
            ((TestProcedure*)obj)->init(mChildEle);
            ((TestProcedure*)obj)->setAutoRunObj(this);

        //	mRunMap.insert(std::map<std::string, CRunObj *>::value_type(strId, obj));

            mRunVec.push_back(std::make_tuple(strId, obj));
        }
    }
}
void autoRun::unInit()
{
    auto itor = mRunVec.begin();

    for (; itor != mRunVec.end(); ++itor)
    {
        auto obj = std::get<ObjIndex>(*itor);

        delete obj;
    }

    std::vector <ValueType> tmp;
    tmp.swap(mRunVec);
}

