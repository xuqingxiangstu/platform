#include "modelmanager.h"
#include <sstream>
#include <QLibrary>

#include "../../PfCommon/tools/ut_error.h"


ModelManager::ModelManager()
{

}

ModelManager::~ModelManager()
{

}


void ModelManager::init(const std::string &xmlPath)
{
    std::stringstream strErr;
    std::stringstream allErr;
    TiXmlDocument mXmlDoc (xmlPath.c_str());

    if(!mXmlDoc.LoadFile())
    {
        strErr << "[ERROR](xml)Open File Error" << xmlPath << "\n";
        throw std::runtime_error(strErr.str());
    }
    else
    {
        TiXmlElement *mRoot = mXmlDoc.RootElement();

        TiXmlElement *mChildEle = mRoot->FirstChildElement();

        for (mChildEle; mChildEle; mChildEle = mChildEle->NextSiblingElement())
        {
            try
            {
                if (mChildEle)
                {
                    std::string strId = mChildEle->Attribute("id");

                    std::string strClass = mChildEle->Attribute("model");

                    auto Itor = mManagement.find(strId);

                    if (Itor != mManagement.end())
                    {
                        strErr.str("");
                        strErr << "[ERROR]CDevManager" << "设备ID(" << strId << ")" << "重复定义";
                        throw std::runtime_error(strErr.str());
                    }

                    bool isLoad = false;                    

#if defined(Q_OS_WIN)
                    std::string dllPath = "./algorithmLib/" + strClass + ".dll";
#else
                    std::string dllPath = "./algorithmLib/lib" + strClass + ".so";
#endif

                    LOAD_STEP_LIB libfun = nullptr;

                    QLibrary lib(dllPath.c_str());//加载*****.dll
                    if (lib.load())//判断是否加载成功
                    {
                        libfun = (LOAD_STEP_LIB)lib.resolve("LoadClass");//获取dll的函数,***为动态库中的函数                                i

                        if(libfun != nullptr)
                        {
                            Algorithm  *initObj = libfun();

                            try
                            {
                                initObj->init(mChildEle);
                                mManagement[strId] = std::shared_ptr<Algorithm>(initObj);
                                isLoad = true;
                            }
                            catch(std::runtime_error err)
                            {
                                throw std::runtime_error(strId + " 初始化失败(" + err.what() + ")");
                            }
                        }
                    }

                    if(!isLoad)
                    {
                        strErr.str("");
                        strErr << "[" << dllPath << "] 加载dll失败:" << strClass;
                        UT_THROW_EXCEPTION(strErr.str());
                    }                    

                }
            }
            catch(std::runtime_error err)
            {
                allErr << err.what() << "\n";
            }
        }

        if(allErr.str() != "")
        {
            throw std::runtime_error(allErr.str());
        }
    }
}

bool ModelManager::getAlgorithm(const std::string &id, Algorithm **algorithm)
{
    bool res = false;

    auto itor = mManagement.find(id);
    if(itor != mManagement.end())
    {
        *algorithm = itor->second.get();
        res = true;
    }

    return res;
}

void ModelManager::setAlgorithm(const std::string &id, Algorithm *algorithm)
{
    mManagement[id] = std::shared_ptr<Algorithm>(algorithm);
}



