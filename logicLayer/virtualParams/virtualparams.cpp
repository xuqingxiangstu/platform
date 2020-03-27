#include "virtualparams.h"
#include <thread>


virtualParams *virtualParams::mInstance = nullptr;
std::mutex virtualParams::mInMutex;

virtualParams::virtualParams()
{
#if 0
    auto beginTime = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::microseconds(5000));
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime= std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);
    int a = elapsedTime.count();
    int b = 0;
#endif

    setValue({"12", "1103", "2"}, mapValue());
    //setValue({"34", "4401", "02"}, mapValue());
}

void virtualParams::setValue(const keyType &key, const valueType &value)
{
    //增加时间戳
    //value.setStartTime(std::chrono::high_resolution_clock::now());


    mParamsManager[key] = value;
}

bool virtualParams::getValue(const keyType &key, valueType &value)
{
    bool res = false;

    auto findItor = mParamsManager.find(key);
    if(findItor != mParamsManager.end())
    {
        value = (findItor->second);
    }

    return res;
}

bool virtualParams::isMeet(const keyType &key)
{
    bool res = false;

    //符合条件后设置为无效

    auto findItor = mParamsManager.find(key);
    if(findItor != mParamsManager.end())
    {
        res = (findItor->second).isMeet();
    }

    return res;
}
