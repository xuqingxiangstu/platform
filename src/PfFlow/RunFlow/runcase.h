#ifndef RUNCASE_H
#define RUNCASE_H

#include "../runObj/runobj.h"

#include <tuple>
#include <vector>
#include <map>
#include <atomic>

#define IF          "ifStep"
#define ELSE_IF     "elseIfStep"
#define ELSE        "elseStep"
#define END         "endStep"
#define BREAK       "breakStep"
#define CONTINUE    "continueStep"
#define WHILE       "whileStep"


class TestCase : public runObj
{
    enum
    {
        KeyIndex = 0,
        ObjIndex
    };

    enum
    {
        If_True,
        If_False,
        Else_If_True,
        Else_If_False
    }runResult;

public:
    TestCase();
    ~TestCase();
public:
    void init(TiXmlElement *);
    void unInit();

    bool exec();

    void exitTest();

    runObj *getObj(std::string stepIndex);
    void setFlowType(std::string type){mCurFlowType = type;}
    void setAutoRunObj(runObj *obj);
    void setMsgObj(Pf::PfAdapter::Adapter *obj){mMsgObj = obj;}
private:
    void updateMsg(const std::string &);
    void run();
    void findPair();
public:
    std::string getDescribe(){ return strDescribe; }
    int getObjSize(){ return mStepObj.size(); }

    std::string getFirstObjName()
    {
        if (mStepObj.size() > 0)
        {
            return std::get<ObjIndex>(mStepObj.at(0))->getClassName();
        }
        return "";
    }

    runObj *getObj(int index)
    {
        if (index >= mStepObj.size())
        {
            return NULL;
        }
        return std::get<ObjIndex>(mStepObj.at(index));
    }

    bool getResult(){ return bRes; }

    std::string outText(){return strOutText;}
private:
    int runIf(int index);
    int runElseIf(int index);
    int runWhile(int index);
    int runElse(int index);
    int runEnd(int index);
    int runBreak(int index);
    int runContinue(int index);
private:
    void checkCondition();
private:
    Pf::PfAdapter::Adapter *mMsgObj;
    using ValueType = std::tuple<std::string, runObj*>;
    std::vector<ValueType> mStepObj;
    std::string strDescribe;
    bool bRes;
    std::atomic_bool isSuspend;
    std::atomic_bool isExit;
    std::atomic_bool isTesting;
    std::string strOutText;
    std::string mCurFlowType;
    using elseType = std::tuple<int, std::vector<std::tuple<std::string, int>>>;
    using ifType = std::tuple<int, elseType>;
    using ifKey = int;
   // std::vector<ifType> mIfVecs;

    std::map<int, int> mWhileVecs;
    std::map<ifKey, elseType> mIfVecs;
};

#endif // RUNCASE_H
