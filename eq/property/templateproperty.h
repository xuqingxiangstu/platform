#ifndef TEMPLATEPROPERTY_H
#define TEMPLATEPROPERTY_H

#include "../../src/PfCommon/jsoncpp/json.h"

#include <map>

class templateProperty
{
public:
    static templateProperty *getInstance()
    {
        if(!mInstance)
        {
            mInstance = new templateProperty();
        }

        return mInstance;
    }
public:
    void init(const std::string &path);
    Json::Value getProperty(const std::string &key);
private:
    templateProperty();
private:
    std::map<std::string, Json::Value> mPropertyObj;
    const std::string mPropertyNodeName = "property";
private:
    static templateProperty *mInstance;

};

#endif // NODEPROPERTY_H
