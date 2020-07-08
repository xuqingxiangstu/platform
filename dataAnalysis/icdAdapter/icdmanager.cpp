#include "icdmanager.h"

std::mutex icdManager::mMutex;
std::shared_ptr<icdManager> icdManager::mInstance = nullptr;

icdManager::icdManager()
{
    mIcdFrameAdpter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();
    mIcdFrameAdpter->init("./cfgfile/icd.xml");

    mFrameTypeMapping[Frame_BE] = FRAME_BE;
    mFrameTypeMapping[Frame_FE] = FRAME_FE;
    mFrameTypeMapping[Frame_Middle] = FRAME_MIDDLE;
}

icdManager::~icdManager()
{

}

std::shared_ptr<PfIcdWorkBench::frameObj> icdManager::getFrameObj(const int &type)
{
    std::shared_ptr<PfIcdWorkBench::frameObj> obj = nullptr;

    if(mFrameTypeMapping.contains(type))
    {
        obj = getFrameObj(mFrameTypeMapping[type].toStdString());
    }

    return obj;
}

std::shared_ptr<PfIcdWorkBench::frameObj> icdManager::getFrameObj(const std::string &key)
{
    return mIcdFrameAdpter->getFrameObj(key);
}

QString icdManager::getMapping(const int &type)
{
    QString keyName = "";

    if(mFrameTypeMapping.contains(type))
    {
        keyName = mFrameTypeMapping[type];
    }

    return keyName;
}

