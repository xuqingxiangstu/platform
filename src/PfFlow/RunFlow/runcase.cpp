#include "runcase.h"

#include <sstream>
#include <stdexcept>

#include <QLibrary>

#include "../../PfCommon/tools/ut_error.h"

TestCase::TestCase()
    :isTesting(false),
    isSuspend(false),
    isExit(false),
    bRes(true),
    mMsgObj(nullptr)
{

}


TestCase::~TestCase()
{    
    auto itor = mStepObj.begin();

    for( ; itor != mStepObj.end();  ++itor)
    {
        auto obj = std::get<ObjIndex>(*itor);

        delete obj;
    }

    std::vector<ValueType> tmp;
    mStepObj.swap(tmp);
}

void TestCase::init(TiXmlElement *xmlEle)
{
    std::ostringstream strErr;

    strDescribe = xmlEle->FirstChildElement(DescribeNode)->GetText();

    TiXmlElement *mChildEle = xmlEle->FirstChildElement(TestStepNode);


    for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement(TestStepNode))
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

            std::string strClass = mChildEle->Attribute(StepTypeAttr);

            bool isLoad = false;

#if defined(Q_OS_WIN)
            std::string dllPath = "./stepLib/" + strClass + ".dll";
#else
            std::string dllPath = "./stepLib/lib" + strClass + ".so";
#endif

            LOAD_STEP_LIB libfun = nullptr;

            QLibrary lib(dllPath.c_str());//加载*****.dll
            if (lib.load())//判断是否加载成功
            {
                libfun = (LOAD_STEP_LIB)lib.resolve("LoadClass");//获取dll的函数,***为动态库中的函数                                i

                if(libfun != nullptr)
                {
                    runObj  *initObj = libfun();

                    initObj->init(mChildEle);

                    mStepObj.push_back(std::make_tuple(strId, initObj));
                    isLoad = true;
                }
            }

            if(!isLoad)
            {
                strErr.str("");
                strErr << "[" << dllPath << "] 加载dll失败:" << strClass;
                UT_THROW_EXCEPTION(strErr.str());
            }
        }
    }

    ///TODO:校验 if-end while-end

    checkCondition();
}

void TestCase::updateMsg(const std::string &msg)
{
    if(msg == "")
        return ;

    ///modify xqx 20191113 增加 "flow": 类型

    std::string tmpMsg;

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

    ///end

    if(mMsgObj)
        mMsgObj->sendMsg(tmpMsg.c_str(), tmpMsg.size());
}

void TestCase::checkCondition()
{
    std::vector<std::string> ifOrWhileVector;
    std::vector<std::string> endVector;

    for (int index = 0; index < mStepObj.size(); index++)
    {
        std::string className = "";

        className = (std::get<ObjIndex>(mStepObj.at(index)))->getClassName();

        if(IF == className)
        {
            ifOrWhileVector.push_back(IF);
        }
        else if(WHILE == className)\
        {
            ifOrWhileVector.push_back(WHILE);
        }
        else if(END == className)
        {
            if(ifOrWhileVector.size() > 0)
            {
                ifOrWhileVector.pop_back();
            }
            else
            {
                UT_THROW_EXCEPTION("end not find if or while!");
            }
        }
        else if( (ELSE_IF == className) || (ELSE == className))
        {
            if(ifOrWhileVector.size() > 0)
            {
                if(IF != ifOrWhileVector.back())
                {
                    UT_THROW_EXCEPTION("ELSE_IF not find if!");
                }
                //ifOrWhileVector.pop_back();
            }
            else
            {
                UT_THROW_EXCEPTION("ELSE_IF is empty!");
            }
        }
    }

    if(!(ifOrWhileVector.size() == 0))
    {
        UT_THROW_EXCEPTION("if or while not find end!");
    }
}

void TestCase::unInit()
{

}


void TestCase::exitTest()
{
    if(isTesting)
        isExit = true;
}

bool TestCase::exec()
{
    isTesting = true;
    bRes = true;
    try
    {       
        run();
    }
    catch(std::runtime_error err)
    {
        bRes = false;
        strOutText = err.what();
    }

    isExit = false;
    isTesting = false;
}

void TestCase::findPair()
{
    std::string className = "";
    std::vector<std::tuple<std::string, int>> ifOrWhileVector;
    std::vector<std::tuple<std::string, int>> elseIfVec;
    ///查找成对if-else if -else-end
    for (int index = 0; index < mStepObj.size(); index++)
    {
        className = (std::get<1>(mStepObj.at(index)))->getClassName();

        if(IF == className)
        {
            ifOrWhileVector.push_back(std::make_tuple(className, index));
        }
        else if(WHILE == className)
        {
            ifOrWhileVector.push_back(std::make_tuple(className, index));
        }
        else if( (ELSE_IF == className) || (ELSE == className))
        {
            elseIfVec.push_back(std::make_tuple(className, index));
        }
        else if(END == className)
        {
            ///找到成对
            if(std::get<0>(ifOrWhileVector.back()) == IF)
            {
                std::vector<std::tuple<std::string, int>> tmp;
                std::copy(elseIfVec.begin(), elseIfVec.end(), std::back_inserter(tmp));
                mIfVecs[std::get<1>(ifOrWhileVector.back())] = std::make_tuple(index, tmp);
                elseIfVec.clear();
                ifOrWhileVector.pop_back();
            }
            else if(std::get<0>(ifOrWhileVector.back()) == WHILE)
            {
                mWhileVecs[std::get<1>(ifOrWhileVector.back())] = index;
                ifOrWhileVector.pop_back();
            }
        }
    }
}

void TestCase::run()
{
    std::string className = "";

    mIfVecs.clear();
    mWhileVecs.clear();
    findPair();

    for (int index = 0; index < mStepObj.size();)
    {
        if (isExit)
        {
            break;
        }

        className = (std::get<ObjIndex>(mStepObj.at(index)))->getClassName();

        if(IF == className)
        {
            index = runIf(index);
        }
        else if(CONTINUE == className)
        {
            index = runContinue(index);
        }
        else if(ELSE_IF == className)
        {
            index = runElseIf(index);
        }
        else if(ELSE == className)
        {
            index = runElse(index);
        }
        else if(WHILE == className)
        {
            index = runWhile(index);
        }
        else if(END == className)
        {
            index = runEnd(index);
        }
        else if(BREAK == className)
        {
            index = runBreak(index);
        }
        else
        {
            (std::get<ObjIndex>(mStepObj.at(index)))->exec();

            updateMsg((std::get<ObjIndex>(mStepObj.at(index)))->outText());

            bRes &= std::get<ObjIndex>(mStepObj.at(index))->getResult();

            index++;
        }
    }
}

void TestCase::setAutoRunObj(runObj *obj)
{
    for(auto item : mStepObj)
    {
        (std::get<ObjIndex>(item))->setAutoRunObj(obj);
    }
}

void TestCase::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    for(auto item : mStepObj)
    {
        (std::get<ObjIndex>(item))->setAdapter(adapter);
    }
}

void TestCase::setIcdframeadapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter)
{
    for(auto item : mStepObj)
    {
        (std::get<ObjIndex>(item))->setIcdframeadapter(icdAdapter);
    }
}

runObj *TestCase::getObj(std::string stepIndex)
{
    auto isFind = std::find_if(mStepObj.begin(), mStepObj.end(), [=](const ValueType &v){
       return  std::get<KeyIndex>(v) == stepIndex;
    });

    if(isFind != mStepObj.end())
    {
        return std::get<ObjIndex>(*isFind);
    }

    return nullptr;
}

int TestCase::runIf(int startIndex)
{    
    int index = startIndex + 1;


    if(!((std::get<ObjIndex>(mStepObj.at(startIndex)))->exec()))    ///如果执行失败
    {
        ///执行下一个else if 或者 else

        if((std::get<1>(mIfVecs[startIndex])).size() == 0)
        {
            ///如果没有else elseif 则直接执行end之后
            index = (std::get<0>(mIfVecs[startIndex])) + 1;
        }
        else
        {
            ///如果有else elseif 则执行下一个else if
            index = std::get<1>((std::get<1>(mIfVecs[startIndex])).at(0));
        }
    }

    return index;
}

int TestCase::runContinue(int startIndex)
{
    int returnIndex = startIndex + 1;

    ///找到break所属的while

    auto itor = std::find_if(mWhileVecs.begin(), mWhileVecs.end(), [=](const std::pair<int, int> &v){
       return (startIndex > v.first) && (startIndex < v.second);
    });

    if(itor != mWhileVecs.end())
    {
        ///返回while 下一个位置
        returnIndex = (*itor).first + 1;
    }

    return returnIndex;
}

int TestCase::runBreak(int startIndex)
{
    int returnIndex = startIndex + 1;

    ///执行成功则找到while对应end跳出循环，否则继续执行
    if(((std::get<ObjIndex>(mStepObj.at(startIndex)))->exec()))    ///执行成功
    {
        ///找到break所属的while

        auto itor = std::find_if(mWhileVecs.begin(), mWhileVecs.end(), [=](const std::pair<int, int> &v){
           return (startIndex > v.first) && (startIndex < v.second);
        });

        if(itor != mWhileVecs.end())
        {
            ///返回end位置下条指令
            returnIndex = (*itor).second + 1;
        }
    }
    return returnIndex;
}

int TestCase::runEnd(int startIndex)
{
    int returnIndex = startIndex + 1;

    ///查看end是否是while，如果是则继续执行while下一条语句，否则继续执行
    auto itor = std::find_if(mWhileVecs.begin(), mWhileVecs.end(), [=](const std::pair<int, int> &v){
       return startIndex == v.second;
    });

    if(itor != mWhileVecs.end())
    {
        ///返回while语句
        returnIndex = (*itor).first;
    }

    return returnIndex;
}

int TestCase::runElse(int startIndex)
{
    int returnIndex = startIndex + 1;
    bool isOk = false;
    int findKey = 0;

    std::vector<std::tuple<std::string, int>>::iterator findItor;

    ///查找对应的if是否执行成功，成功则直接执行end之后，失败则执行此elseif
    for(auto itor = mIfVecs.begin(); itor != mIfVecs.end(); itor++)
    {

        findItor = std::find_if( (std::get<1>(itor->second)).begin(), (std::get<1>(itor->second)).end(), [=](const std::tuple<std::string, int> &v){
           return  std::get<1>(v) == startIndex;
        });

        if(findItor != (std::get<1>(itor->second)).end())
        {
            findKey = itor->first;

            if((std::get<ObjIndex>(mStepObj.at(itor->first)))->getResult())
            {
                isOk = true;
                returnIndex = std::get<0>(itor->second);
            }
            break;
        }
    }

    if(!isOk)
    {
        ///如果为第一个else则直接执行，否则先判断上一个else if执行是否成功
        if(findItor == (std::get<1>(mIfVecs[findKey])).begin())
        {
            ///执行else
            if(!((std::get<ObjIndex>(mStepObj.at(startIndex)))->exec()))    ///如果执行失败
            {
                returnIndex = std::get<0>(mIfVecs[findKey]);
            }
        }
        else
        {
            findItor--;

            if(((std::get<ObjIndex>(mStepObj.at(std::get<1>(*findItor))))->getResult()))
            {
                ///成功则直接跳到end
                returnIndex = (std::get<0>(mIfVecs[findKey])) + 1;
            }
            else
            {
                ///失败则执行下一个else if / else
                findItor++;
                ///执行else
                if(!((std::get<ObjIndex>(mStepObj.at(startIndex)))->exec()))    ///如果执行失败
                {
                    returnIndex = std::get<0>(mIfVecs[findKey]);
                }
            }
        }
    }

    return returnIndex;
}

int TestCase::runElseIf(int startIndex)
{
    int returnIndex = startIndex + 1;
    bool isOk = false;
    int findKey = 0;
    int findValueIndex = 0;

    std::vector<std::tuple<std::string, int>>::iterator findItor;

    ///查找对应的if是否执行成功，成功则直接执行end之后，失败则执行此elseif
    for(auto itor = mIfVecs.begin(); itor != mIfVecs.end(); itor++)
    {

        findItor = std::find_if( (std::get<1>(itor->second)).begin(), (std::get<1>(itor->second)).end(), [=](const std::tuple<std::string, int> &v){
           return  std::get<1>(v) == startIndex;
        });

        if(findItor != (std::get<1>(itor->second)).end())
        {
            findKey = itor->first;

            if((std::get<ObjIndex>(mStepObj.at(itor->first)))->getResult())
            {
                isOk = true;
                returnIndex = std::get<0>(itor->second);
            }
            break;
        }
    }

    if(!isOk)
    {
        ///如果为第一个else if则直接执行，否则先判断上一个else if执行是否成功
        if(findItor == (std::get<1>(mIfVecs[findKey])).begin())
        {
            ///执行else if / else
            if(!((std::get<ObjIndex>(mStepObj.at(startIndex)))->exec()))    ///如果执行失败
            {
                ///没有节点则直接执行end
                if(findItor == (std::get<1>(mIfVecs[findKey])).end())
                {
                    returnIndex = std::get<0>(mIfVecs[findKey]);
                }
                else
                {
                    findItor++;

                    returnIndex = std::get<1>(*findItor);
                }
            }
        }
        else
        {
            findItor--;

            if(!((std::get<ObjIndex>(mStepObj.at(std::get<1>(*findItor))))->getResult()))
            {
                ///失败则执行下一个else if / else
                findItor++;

                ///执行else if / else
                if(!((std::get<ObjIndex>(mStepObj.at(std::get<1>(*findItor))))->exec()))    ///如果执行失败
                {
                    ///没有节点则直接执行end
                    if(findItor == (std::get<1>(mIfVecs[findKey])).end())
                    {
                        returnIndex = std::get<0>(mIfVecs[findKey]);
                    }
                    else
                    {
                        findItor++;

                        returnIndex = std::get<1>(*findItor);
                    }
                }
            }
            else
            {
                ///if 执行成功则直接执行 end
                returnIndex = std::get<0>(mIfVecs[findKey]);
            }
        }
    }

    return returnIndex;
}

int TestCase::runWhile(int startIndex)
{
    int returnIndex = startIndex + 1;

    ///执行成功则继续运行，否则跳出循环（END）
    if(!((std::get<ObjIndex>(mStepObj.at(startIndex)))->exec()))    ///执行失败
    {
        ///找到所属while
        auto itor = std::find_if(mWhileVecs.begin(), mWhileVecs.end(), [=](const std::pair<int, int> &v){
           return (startIndex >= v.first) && (startIndex <= v.second);
        });

        if(itor != mWhileVecs.end())
        {
            ///返回end位置下条指令
            returnIndex = (*itor).second + 1;
        }
    }

    return returnIndex;
}
