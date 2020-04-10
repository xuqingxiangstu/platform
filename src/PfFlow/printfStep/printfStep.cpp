#include "printfStep.h"

#include "../../PfCommon/tools/ut_error.h"

#include "../condition/condition.h"

#include <regex>

printfStep::printfStep():
mRunObj(nullptr),
mAdapter(nullptr),
strOutText(),
mResult(false)
{
}

printfStep::~printfStep()
{

}

void printfStep::init(const TiXmlElement *xmlEle)
{
    const TiXmlElement *pTmpElem = NULL;

    pTmpElem = xmlEle->FirstChildElement("Printf");
    if (pTmpElem)
    {
        const char *point = nullptr;
        point = pTmpElem->GetText();
        if(point)
            mFormat = point;
    }

    pTmpElem = xmlEle->FirstChildElement("Argu1");
    if (pTmpElem)
    {
        const char *point = nullptr;
        point = pTmpElem->GetText();
        if(point)
            strArgs.push_back(point);
    }

    pTmpElem = xmlEle->FirstChildElement("Argu2");
    if (pTmpElem)
    {
        const char *point = nullptr;
        point = pTmpElem->GetText();
        if(point)
            strArgs.push_back(point);
    }

    pTmpElem = xmlEle->FirstChildElement("Argu3");
    if (pTmpElem)
    {
        const char *point = nullptr;
        point = pTmpElem->GetText();
        if(point)
            strArgs.push_back(point);
    }

    pTmpElem = xmlEle->FirstChildElement("Argu4");
    if (pTmpElem)
    {
        const char *point = nullptr;
        point = pTmpElem->GetText();
        if(point)
            strArgs.push_back(point);
    }

    pTmpElem = xmlEle->FirstChildElement("MessageType");
    if (pTmpElem)
    {
        const char *point = nullptr;
        point = pTmpElem->GetText();
        if(point)
            msgType = point;
    }
}

std::string printfStep::getClassName()
{
    return VAR_NAME(printfStep);
}

bool printfStep::exec()
{
    std::ostringstream showMsg;
    showMsg.str();
    std::string strFormat = mFormat;
    memset(strTmpBuf, 0, BUF_SIZE * sizeof(char));
    std::regex sReg(".*(%\\d*\\.?\\d*\\s*s).*", std::regex::icase);
    std::regex fReg(".*(%\\d*\\.?\\d*\\s*f).*", std::regex::icase);
    std::regex dReg(".*(%\\d*\\.?\\d*\\s*d).*", std::regex::icase);

    int argIndex = 0;

    if(argIndex >= strArgs.size())
        showMsg << strFormat;

    while(argIndex < strArgs.size())
    {
        std::string::size_type fSize = strFormat.find_first_of('%');

        if(fSize == std::string::npos)
        {
            showMsg << strFormat;
            break;
        }

        ///剔除转移

        if( (fSize > 0) && (strFormat.at(fSize - 1) == '\\'))
        {
            showMsg << strFormat.substr(0, fSize + 1);
            strFormat = strFormat.substr(fSize + 1, strFormat.size() - fSize - 1);
            continue;
        }

        std::string symbol;

        ///找到下一个%
        std::string::size_type sSize = strFormat.substr(fSize + 1, strFormat.size() - 1 - fSize).find_first_of('%');
        if(sSize == std::string::npos)
        {
            symbol = strFormat;
        }
        else
        {
            symbol = strFormat.substr(0, fSize + sSize + 1);
            strFormat = strFormat.substr(sSize + fSize + 1, strFormat.size() - symbol.size());
        }

        std::smatch matchRes;
        std::string varValue = "";

        ///判断格式类型
        if(std::regex_match(symbol, sReg))  ///< %s
        {
            varValue = getVarV(strArgs[argIndex]);
            if(varValue != "")
                sprintf(strTmpBuf, symbol.c_str(), varValue.c_str());
            else
                sprintf(strTmpBuf, symbol.c_str(), strArgs[argIndex].c_str());

            argIndex++;
        }
        else if(std::regex_match(symbol, dReg))  ///< %d
        {
            varValue = getVarV(strArgs[argIndex]);
            if(varValue != "")
                sprintf(strTmpBuf, symbol.c_str(), std::atoi(varValue.c_str()));
            else
                sprintf(strTmpBuf, symbol.c_str(), std::atoi(strArgs[argIndex].c_str()));

            argIndex++;
        }
        else if(std::regex_match(symbol, fReg))  ///< %f
        {
            varValue = getVarV(strArgs[argIndex]);
            if(varValue != "")
                sprintf(strTmpBuf, symbol.c_str(), std::atof(varValue.c_str()));
            else
                sprintf(strTmpBuf, symbol.c_str(), std::atof(strArgs[argIndex].c_str()));

            argIndex++;
        }

        showMsg << std::string(strTmpBuf);

        memset(strTmpBuf, 0, BUF_SIZE * sizeof(char));
    }

    toUi(showMsg.str(), msgType);

#ifndef QT_NO_DEBUG
    UT_SHOW(showMsg.str());
#endif
    mResult = true;
    return mResult;
}
std::string printfStep::getVarValue(std::string)
{
    return "";
}

std::string printfStep::getVarV(std::string use)
{
    Condition cond;

    return cond.getVarValue(mRunObj, use);;
}

void printfStep::toUi(std::string msg, std::string status)
{
    strOutText = "msgType\":\"cmd\",\"msg\":{\"type\":\"showMsg\",\"status\":\"" + status + "\"\"show\":\"" + msg +"\"}}";
}

bool printfStep::getResult()
{
    return mResult;
}

std::string printfStep::outText()
{
    return strOutText;
}

void printfStep::setAutoRunObj(runObj *obj)
{
    mRunObj = obj;
}

void printfStep::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)
{
    mAdapter = adapter;
}

extern "C"
{
    runObj *LoadClass()
    {
        return new printfStep();
    }
}

    
    
