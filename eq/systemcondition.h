#ifndef SYSTEMCONDITION_H
#define SYSTEMCONDITION_H

#include <memory>
#include <QStringList>
#include <QString>

#include "../src/PfCommon/jsoncpp/json.h"

/**
 * @brief The systemCondition class 系统条件
 */

class systemCondition
{
public:
    static std::shared_ptr<systemCondition> getInstance()
    {
        if(mInstance == nullptr)
        {
            mInstance = std::make_shared<systemCondition>();
        }

        return mInstance;
    }
public:
    systemCondition();
    ~systemCondition();
public:
    void getInitValue(Json::Value &value);
private:
    Json::Value mConditionInitValue;
private:
    static std::shared_ptr<systemCondition> mInstance;
};

#endif // SYSTEMCONDITION_H
