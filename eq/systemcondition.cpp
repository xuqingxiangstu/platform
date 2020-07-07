#include "systemcondition.h"

#include "../src/PfSql/paramsTable/systemtable.h"
#include "../src/PfSql/paramsTable/sysinterfacetable.h"

#include "./property/nodeproperty.h"

std::shared_ptr<systemCondition> systemCondition::mInstance = nullptr;

systemCondition::systemCondition()
{
    //更新目标设备记录
    Json::Value interfaceJs = sysInterfaceTable::getInstance()->getValue();

    Json::Value arrayJs;

    Json::Value uiJs;

    uiJs[PROPERTY_DEV_VALUE_UUID] = Json::Value("UI");
    uiJs[PROPERTY_DEV_VALUE_NAME] = Json::Value("UI");

    //arrayJs.append(uiJs);

    for(int tmpIndex = 0; tmpIndex < interfaceJs.size(); tmpIndex++)
    {
        Json::Value tmpJs;

        tmpJs[PROPERTY_DEV_VALUE_UUID] = interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_UUID].asString();
        tmpJs[PROPERTY_DEV_VALUE_NAME] = interfaceJs[tmpIndex][SYSTEM_INTERFACE_TABLE_DEV_NAME].asString();

        arrayJs.append(tmpJs);
    }

    mConditionInitValue = arrayJs;
}

systemCondition::~systemCondition()
{

}

void systemCondition::getInitValue(Json::Value &value)
{
    value = mConditionInitValue;
}
