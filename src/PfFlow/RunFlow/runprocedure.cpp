#include "runprocedure.h"

#include <sstream>
#include <stdexcept>
#include <vector>
#include "../../PfCommon/tools/ut_error.h"
#include "../../PfCommon/cmdToJson/cmdtojson.h"
using namespace Pf;
TestProcedure::TestProcedure()
    :isTesting(false),
    mCurTestIndex(0),
    isSuspend(false),
    isExit(false),
    isRuning(false),
    mTestResult(true),
    mMsgObj(nullptr)
{
}


TestProcedure::~TestProcedure()
{
    auto itor = mTestCaseVec.begin();

    for( ; itor != mTestCaseVec.end();  ++itor)
    {
        auto obj = std::get<ObjIndex>(*itor);

        delete obj;
    }

    std::vector<ValueType> tmp;
    mTestCaseVec.swap(tmp);
}

void TestProcedure::init(TiXmlElement *xmlEle)
{
    std::ostringstream strErr;

    strDescribe = xmlEle->FirstChildElement(DescribeNode)->GetText();

    TiXmlElement *mChildEle = xmlEle->FirstChildElement(TestCaseNode);

    for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement(TestCaseNode))
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

            TestCase *obj = new TestCase();
            obj->init(mChildEle);

            mTestCaseVec.push_back(std::make_tuple(strId, obj));
        }
    }
}
void TestProcedure::unInit()
{

}

bool TestProcedure::exec()
{   
    if(!isRuning)
    {
        isRuning = true;
        isExit = false;
        isSuspend = false;
        mTestResult = true;

        run();

        isRuning = false;
        isExit = false;
        isSuspend = false;
    }

    return true;
}

//暂停测试
void  TestProcedure::suspendTest()
{
    isSuspend = true;
}
//继续测试
void  TestProcedure::continueTest()
{
    //if (isTesting)
    if(isSuspend)
    {
        std::unique_lock<std::mutex> lk(mMutex);
        mCv.notify_one();
    }
}

void TestProcedure::exitTest()
{
    if (isRuning)
    {
        isExit = true;

        //TODO:
        for (unsigned int i = 0; i < mTestCaseVec.size(); i++)
        {
            dynamic_cast<TestCase*>(std::get<ObjIndex>(mTestCaseVec.at(i)))->exitTest();
        }
    }
}

void TestProcedure::updateMsg(const std::string &msg)
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

void TestProcedure::run()
{
    std::vector<std::string> vecCode;
    int i32Row = 0;
    std::string strMsg;
    for (unsigned int i = 0; i < mTestCaseVec.size(); i++)
    {
        std::string strDes = (std::get<ObjIndex>(mTestCaseVec.at(i)))->getDescribe();
        //TODO:向UI发送
        vecCode.clear();
        vecCode.push_back("showMsg");
        vecCode.push_back("normal");
        vecCode.push_back(strDes);
        strMsg=PfCommon::cmdToJson::replyCtrlCmd(vecCode,true);
        updateMsg(strMsg);

        //std::string strClassName = (std::get<ObjIndex>(mTestCaseVec.at(i)))->getFirstObjName();
        strMsg="";
        std::tuple<std::string ,int ,std::string> param(strFatherId,i32Row,"test");
        strMsg=PfCommon::cmdToJson::replyTestStatusCmd("normal",param);
        updateMsg(strMsg);

        isTesting = true;

        //退出
        if (isExit)
        {
            break;
        }

        //暂停
        if(isSuspend)
        {
            std::unique_lock<std::mutex> lkw(mMutex);
            mCv.wait(lkw);

            isSuspend = false;
        }

        //退出
        if (isExit)
        {
            break;
        }

        std::cout << strDes;
        //执行
        (std::get<ObjIndex>(mTestCaseVec.at(i)))->exec();

        notify((std::get<ObjIndex>(mTestCaseVec.at(i))));

        //退出
        if (isExit)
        {
            break;
        }

        bool res = (std::get<1>(mTestCaseVec.at(i)))->getResult();

        mTestResult &= res;

        //退出
        if (isExit)
        {
            break;
        }

        //向UI发送测试结果

        strMsg="";
        if (res)
        {
            std::tuple<std::string ,int ,std::string> param(strFatherId,i32Row,"normal");
            strMsg=PfCommon::cmdToJson::replyTestStatusCmd("normal",param);
        }
        else
        {
            std::tuple<std::string ,int ,std::string> param(strFatherId,i32Row,"error");
            strMsg=PfCommon::cmdToJson::replyTestStatusCmd("normal",param);

        }

        updateMsg(strMsg);

        if(!res)
        {
            strMsg="";
            vecCode.clear();
            vecCode.push_back("showMsg");
            vecCode.push_back("normal");
            vecCode.push_back((std::get<ObjIndex>(mTestCaseVec.at(i)))->outText());
            strMsg=PfCommon::cmdToJson::replyCtrlCmd(vecCode,false);
            updateMsg(strMsg);
        }

        //TODO:异常提示，继续还是退出，还是重复测试
        /*if (!res)
        {
            strCode.str("");
            strCode << "{\"msgType\":\"errInfo\",\"param\":{\"errMsg\":\"" << mTestCaseVec.at(i)->outText() << "\"}}";
            messageDispose::getInstance()->sendMsg(strCode.str());

            std::string rMsg = messageDispose::getInstance()->receiveMsg();

            if(rMsg == "abort")
            {
                isExit = false;
                isRuning = false;
                isTesting = false;
                mTestResult = false;
                throw std::runtime_error("abort");
            }
            else if(rMsg == "retry")
            {
                i--;
                continue;
            }
            else if(rMsg == "ignore")
            {

            }
        }*/

        i32Row++;
    }
#ifdef DEBUG_VER
    printMutex.lock();
    std::cout << "testProcedure stop thread id = " << std::this_thread::get_id() << "\r\n";
    printMutex.unlock();
#endif

    if (isExit)
    {
        std::tuple<std::string ,int ,std::string> param(strFatherId,i32Row,"error");
        strMsg=PfCommon::cmdToJson::replyTestStatusCmd("normal",param);
        updateMsg(strMsg);

        mTestResult = false;


    }

    if(mTestResult)
    {
        std::tuple<std::string ,int ,std::string> param(strFatherId,-1,"normal");
        strMsg=PfCommon::cmdToJson::replyTestStatusCmd("normal",param);

    }
    else
    {
        std::tuple<std::string ,int ,std::string> param(strFatherId,-1,"error");
        strMsg=PfCommon::cmdToJson::replyTestStatusCmd("normal",param);
    }
    updateMsg(strMsg);
}

void TestProcedure::getRunItems(std::vector<std::string> &items)
{
    for (unsigned int i = 0; i < mTestCaseVec.size(); i++)
    {
        items.push_back((std::get<ObjIndex>(mTestCaseVec.at(i)))->getDescribe());
    }
}

void TestProcedure::setMsgObj(Pf::PfAdapter::Adapter *obj)
{
    mMsgObj = obj;

    for (unsigned int i = 0; i < mTestCaseVec.size(); i++)
    {
        (dynamic_cast<TestCase*>(std::get<ObjIndex>(mTestCaseVec.at(i))))->setMsgObj(obj);
    }
}

void TestProcedure::setAutoRunObj(runObj *obj)
{
    for (unsigned int i = 0; i < mTestCaseVec.size(); i++)
    {
        (dynamic_cast<TestCase*>(std::get<ObjIndex>(mTestCaseVec.at(i))))->setAutoRunObj(obj);
    }
}

void TestProcedure::setFlowType(std::string type)
{
    mCurFlowType = type;

    for (unsigned int i = 0; i < mTestCaseVec.size(); i++)
    {
        (dynamic_cast<TestCase*>(std::get<ObjIndex>(mTestCaseVec.at(i))))->setFlowType(type);
    }
}

runObj *TestProcedure::getObj(std::string testCaseIndex, std::string stepIndex)
{
    auto isFind = std::find_if(mTestCaseVec.begin(), mTestCaseVec.end(), [=](const ValueType &v){
       return  std::get<KeyIndex>(v) == testCaseIndex;
    });

    if(isFind != mTestCaseVec.end())
    {
        return dynamic_cast<TestCase*>(std::get<ObjIndex>(*isFind))->getObj(stepIndex);
    }

    return nullptr;
}

void TestProcedure::notify(TestCase *testCase)
{
    //TODO:信息输出
    std::cout << testCase->outText() << std::endl;
}
