#ifndef ACTIVETIME_H
#define ACTIVETIME_H

#include <memory>
#include <QString>

/**
 * @brief The activeTime class   参数有效时间配置
 */
class activeTime
{
public:
    static std::shared_ptr<activeTime> getInstance()
    {
        if(mInstance == nullptr)
        {
            mInstance = std::make_shared<activeTime>();
        }

        return mInstance;
    }

    activeTime();
    ~activeTime();
public:
    int getActiveTime(){return mActiveTime;}
private:
    int mActiveTime;
    const QString mFilePath = "./cfgfile/paramActiveTime.json";
private:
    static std::shared_ptr<activeTime> mInstance;
};

#endif // ACTIVETIME_H
