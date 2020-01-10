#ifndef CONDITION_H
#define CONDITION_H

/**
 * @file condition.h
 * @brief 条件判断，与或非等
 * @author xqx
 * @date 2020-01-09
 * @version v1.0
 */

#include "condition_global.h"

#include "../runObj/runobj.h"

#include "../../PfCommon/TinyXml/tinyxml.h"

/**
 * @brief The Condition class
 * 条件判断类
 */
class CONDITIONSHARED_EXPORT Condition
{
public:
    Condition();
public:
    void init(const TiXmlElement *);
    void setAutoRunObj(runObj *obj){mRunobj = obj;}
    bool exec();
    std::string getVarValue(runObj *obj, std::string use);
private:
    std::string strV;
    std::string strCalc;
    bool mResult;
    runObj *mRunobj;
};

#endif // CONDITION_H
