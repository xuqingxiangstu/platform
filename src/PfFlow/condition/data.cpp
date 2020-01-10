#include "data.h"

#include <regex>
#include<cstdio>

bool calcFactory::judge(const std::string &symbool, const dataValueType &inV, const dataValueType &bV)
{
    bool res = false;

    if(B_LESS == symbool)
    {
        if(inV < bV)
        {
            res = true;
        }
    }
    else if(B_LESS_EQUAL == symbool)
    {
        if(inV <= bV)
        {
            res = true;
        }
    }
    else if(B_EQUAL == symbool)
    {
        if(inV == bV)
        {
            res = true;
        }
    }
    else if(B_THAN == symbool)
    {
        if(inV > bV)
        {
            res = true;
        }
    }
    else if(B_THAN_EQUAL == symbool)
    {
        if(inV >= bV)
        {
            res = true;
        }
    }

    return res;
}

bool calcFactory::calc(const std::string &condition, const dataValueType &inV)
{
    bool res = false;
    std::string symbool1 = "", symbool2 = "";
    dataValueType bV1 = 0, bV2 = 0;
    std::string bWith = B_WITH;

    std::string::size_type index = condition.find(bWith);
    if(std::string::npos != index)
    {
        ///与计算，分别获取两组数据
        std::string tmp = condition.substr(0, index);
        data::getSymboolAndBV(tmp, symbool1, bV1);

        tmp = condition.substr(index + bWith.size(), condition.size() - index - bWith.size());

        if(data::getSymboolAndBV(tmp, symbool2, bV2))
            res = judge(symbool1, inV, bV1) && judge(symbool2, inV, bV2);
    }
    else
    {
        std::string tmp = condition;

        if(data::getSymboolAndBV(tmp, symbool1, bV1))
            res = judge(symbool1, inV, bV1);
    }


    return res;
}

bool data::isBeyondSymboolOk(const std::string &condition)
{
    bool res = true;
    std::regex reg("^\\s*v\\s*(>|>=|<|<=|==)\\s*(\\d+)\\s*$");

    std::string bWith = B_WITH;

    std::string::size_type index = condition.find(bWith);
    if(std::string::npos != index)
    {
        std::string tmp1 = condition.substr(0, index);

        std::string tmp2 = condition.substr(index + bWith.size(), condition.size() - index - bWith.size());

        res = std::regex_match(tmp1, reg) && std::regex_match(tmp2, reg);
    }
    else
    {
        std::string tmp = condition;
        res = std::regex_match(tmp, reg);
    }

    return res;
}

bool data::getSymboolAndBV(const std::string &src, std::string &symbool, dataValueType &BV)
{
    bool res = false;

    std::regex reg("^\\s*v\\s*(>|>=|<|<=|==)\\s*(\\d+)\\s*$", std::regex::icase);
    std::smatch matchRes;

    if(std::regex_match(src, matchRes, reg))
    {    
        if(matchRes.size() == 3)
        {
            int index = 1;
            symbool = matchRes[index++];
            if(converDigital(matchRes[index++], BV))
            {
                res = true;
            }
        }
    }

    return res;
}

bool data::converDigital(const std::string &src, dataValueType &value)
{
    bool isOk = false;

    if(src == "")
    {
        value = 0;
        return true;
    }

    dataType type = isDigital(src, isOk);
    if(isOk)
    {
        if(type == Hex)
        {
            int tmp = 0;
            sscanf(src.c_str(), "%x", &tmp);
            value = tmp;
        }
        else if(type == Dec)
        {
            int tmp = 0;
            sscanf(src.c_str(), "%d", &tmp);
            value = tmp;
        }
        else if(type == Float)
        {
            double tmp = 0;
            sscanf(src.c_str(), "%lf", &tmp);
            value = tmp;
        }
    }

    return isOk;
}

data::dataType data::isDigital(const std::string &src, bool &isOk)
{
    dataType type;

    isOk = false;

    ///校验是否是十六进制
    std::regex hexReg("^[0][x][0-9a-fA-F]+$", std::regex::icase);

    if(std::regex_match(src, hexReg))
    {
        isOk = true;
        type = Hex;
    }

    ///校验是否是十进制
    if(!isOk)
    {
        if(std::regex_match(src, std::regex("^\\s*-*\\d+\\s*$", std::regex::icase)))
        {
            isOk = true;
            type = Dec;
        }
    }

    ///校验是否是浮点数
    if(!isOk)
    {
        if(std::regex_match(src, std::regex("^(-?\\d+)(\\.\\d+)?$", std::regex::icase)))
        {
            isOk = true;
            type = Float;
        }
    }

    return type;
}

bool data::getIndexFromV(const std::string &src, std::string &level1Index, std::string &level2Index, std::string &level3Index, std::string &var, std::string &value)
{
    bool isParse = false;

    ///!1@2#23$Value$4
    std::regex reg("^!(\\d+)@(\\d+)#(\\d+)\\$(\\w+)\\$(\\d+)", std::regex::icase);
    std::smatch matchRes;
    isParse = std::regex_match(src, matchRes, reg);

    if(isParse)
    {
        if(matchRes.size() != 6)
        {
            isParse = false;
        }
        int index = 1;

        level1Index = matchRes[index++];
        level2Index = matchRes[index++];
        level3Index = matchRes[index++];
        var = matchRes[index++];
        value = matchRes[index++];
    }

    return isParse;
}



bool data::isOkCalcFormat(const std::string &src)
{
    bool res = false;

    ///暂支持 v >/>=/</<=/== 数字 && v >/>=/</<=/== 数字
    std::string bWith = B_WITH;

    std::string::size_type index = src.find(bWith);
    if(std::string::npos == index)
    {
        res = isBeyondSymboolOk(src);
    }
    else
    {
        std::string tmp1 = src.substr(0, index);

        std::string tmp2 = src.substr(index + bWith.size(), src.size() - index - bWith.size());

        res = isBeyondSymboolOk(tmp1) && isBeyondSymboolOk(tmp2);
    }

    return res;
}
