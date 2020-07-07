#include "templateproperty.h"

#include <fstream>

#include "../../src/PfCommon/tools/ut_error.h"

#include <QDebug>

templateProperty *templateProperty::mInstance = nullptr;

templateProperty::templateProperty()
{

}

void templateProperty::init(const std::string &path)
{
    std::ostringstream errInfo;

    std::ifstream ifs;
    ifs.open(path);
    if(!ifs.is_open())
    {
        errInfo << "[ERROR] open file(" << path << ") faild";
        UT_THROW_EXCEPTION(errInfo.str());
    }

    Json::Value value;
    Json::Reader jsonReader;
    if (!jsonReader.parse(ifs, value))
    {
        errInfo << "[ERROR] parse json (" << path << ") faild";
        UT_THROW_EXCEPTION(errInfo.str());
    }

    Json::Value configJs = value["config"];
    if(configJs.isNull())
    {
        errInfo << "[ERROR] get config faild";
        UT_THROW_EXCEPTION(errInfo.str());
    }

    for(int index = 0; index < configJs.size(); index++)
    {
        std::string key = configJs[index]["key"].asString();

        Json::Value value = configJs[index];

        mPropertyObj[key] = value;
    }    
}
Json::Value templateProperty::getProperty(const std::string &key)
{
    auto itor = mPropertyObj.find(key);
    if(itor != mPropertyObj.end())
    {
        return itor->second;
    }

    return nullptr;
}
