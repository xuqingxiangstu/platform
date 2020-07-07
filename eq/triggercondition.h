#ifndef TRIGGERCONDITION_H
#define TRIGGERCONDITION_H

#include <memory>
#include <QStringList>
#include <QString>

#include "../src/PfCommon/jsoncpp/json.h"

/**
 * @brief The triggerCondition class    触发条件管理，初始化及获取条件
 */
class triggerCondition
{
public:
    static std::shared_ptr<triggerCondition> getInstance()
    {
        if(mInstance == nullptr)
        {
            mInstance = std::make_shared<triggerCondition>();
        }

        return mInstance;
    }

public:
    triggerCondition();
    ~triggerCondition();
public:
    void getInitValue(Json::Value &value);
private:
    QStringList getValueMeans(QString value);
private:
    Json::Value mConditionInitValue;
private:
    static std::shared_ptr<triggerCondition> mInstance;
};

#endif // TRIGGERCONDITION_H
