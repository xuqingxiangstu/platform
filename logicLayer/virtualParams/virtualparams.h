#ifndef VIRTUALPARAMS_H
#define VIRTUALPARAMS_H

/**
 * @brief The virtualParams class 虚拟参数
 */

#include <unordered_map>
#include <mutex>
#include <string>
#include <chrono>

class mapKey
{
public:
    mapKey(std::string f, std::string s, std::string t):first(f), second(s), three(t){}
public:
    std::string first;
    std::string second;
    std::string three;
};

class mapValue
{
public:
    mapValue():isValid(true){}
    mapValue(std::string v):mValue(v),isValid(true){}

    //void setStartTime(std::chrono::time_point time){startTime = time;}

    std::string getValue(){return mValue;}

    bool isMeet(){
        bool res = isValid;
        isValid = false;
        return res;
    }

    void setInValid(){isValid = false;}
private:
   // std::chrono::time_point startTime;
    std::string mValue;
    bool isValid;
};

class mapHash
{
public:
    std::size_t operator()(const mapKey &key) const
    {
        using std::size_t;
        using std::hash;

        //modify xqx 20200410 不判设备UUID
        return ((hash<std::string>()(key.first)
            ^ (hash<std::string>()(key.second) << 1)) >> 1)
            ^ (hash<std::string>()(key.three) << 1);

        //return ((hash<std::string>()(key.second)
         //   ^ (hash<std::string>()(key.three) << 1)) >> 1);
    }
};

class mapEqualTo
{
public:
    bool operator()(const mapKey& key1, const mapKey& key2) const
    {
        //modify xqx 20200410 不判设备UUID
        return (key1.first == key2.first) && (key1.second == key2.second) && (key1.three == key2.three);
       // return (key1.second == key2.second) && (key1.three == key2.three);
    }
};

class virtualParams
{
public:
    static virtualParams *getInstance()
    {
        if(mInstance == nullptr)
        {
            mInMutex.lock();
            if(mInstance == nullptr)
            {
                mInstance = new virtualParams();
            }
            mInMutex.unlock();
        }

        return mInstance;
    }
private:
    using keyType = mapKey;
    using valueType = mapValue;
private:
    virtualParams();
public:
    void setValue(const keyType &key, const valueType &value);
    bool getValue(const keyType &key, valueType &value);
    bool isMeet(const keyType &key);
private:

    std::unordered_map<keyType, valueType, mapHash, mapEqualTo> mParamsManager;

private:
    static virtualParams *mInstance;
    static std::mutex mInMutex;
};

#endif // VIRTUALPARAMS_H
