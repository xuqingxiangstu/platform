#include "pfadaptermanager.h"

#include <sstream>
#include <QLibrary>
#include <QDir>
#include <QFile>
#include "../../PfCommon/jsoncpp/json.h"
#include "../../PfCommon/tools/ut_error.h"

namespace Pf
{
    namespace PfAdapter
    {
        PfAdapterManager::PfAdapterManager()
        {

        }

        PfAdapterManager::~PfAdapterManager()
        {
            for(auto itor = mManagement.begin(); itor != mManagement.end();)
            {                
                mManagement.erase(itor++);
            }
        }

        void PfAdapterManager::initPassJsonFile(const std::string &filePath)
        {
            QFile fileObj(QString::fromStdString(filePath));
            if(fileObj.open(QIODevice::ReadOnly))
            {
                std::string fileValue = fileObj.readAll().toStdString();
                fileObj.close();

                initPassJsonStr(fileValue);
            }
            else
            {
                UT_THROW_EXCEPTION("[ERROR] open file faild => " + filePath);
            }
        }

        void PfAdapterManager::initPassJsonStr(const std::string &json)
        {
            std::stringstream allErr;
            std::ostringstream strErr;
            //加载JSon
            Json::Reader reader;
            Json::Value root;
            if(!reader.parse(json, root))
            {
                strErr.str("");
                strErr << "json格式错误(" << json << ")";

                UT_THROW_EXCEPTION(strErr.str());
            }

            Json::Value cfgJs = root["configure"];
            if(cfgJs.isNull())
            {
                strErr.str("");
                strErr << "get configure key faild (" << json << ")";

                UT_THROW_EXCEPTION(strErr.str());
            }

            if(!cfgJs.isArray())
            {
                strErr.str("");
                strErr << "the key configure is not array (" << json << ")";

                UT_THROW_EXCEPTION(strErr.str());
            }

            for(int index = 0; index < cfgJs.size(); index++)
            {
                Json::Value node = cfgJs[index];

                try
                {
                    if(!node["uuid"].isNull() && !node["class"].isNull() && !node["params"].isNull())
                    {
                        std::string strId = node["uuid"].asString();
                        std::string strClass = node["class"].asString();
                        Json::Value paramsJs = node["params"];

                        auto Itor = mManagement.find(strId);

                        if (Itor != mManagement.end())
                        {
                            strErr.str("");
                            strErr << "[ERROR]CDevManager" << "设备ID(" << strId << ")" << "重复定义";
                            throw std::runtime_error(strErr.str());
                        }

                        bool isLoad = false;

#if defined(Q_OS_WIN)
                        std::string dllPath = "./adapterLib/" + strClass + ".dll";
#else
                        std::string dllPath = "./adapterLib/lib" + strClass + ".so";
#endif

                        LOAD_STEP_LIB libfun = nullptr;

                        QLibrary lib(dllPath.c_str());//加载*****.dll
                        if (lib.load())//判断是否加载成功
                        {
                            libfun = (LOAD_STEP_LIB)lib.resolve("LoadClass");//获取dll的函数,***为动态库中的函数                                i

                            if(libfun != nullptr)
                            {
                                Adapter  *initObj = libfun();

                                try
                                {
                                    initObj->init(paramsJs);
                                    mManagement[strId] = std::shared_ptr<Adapter>(initObj);
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
                            strErr << "[" << dllPath << "] 加载dll失败:" << strClass << "(" + lib.errorString().toStdString() + ")";
                            UT_THROW_EXCEPTION(strErr.str());
                        }

                    }
                    else
                    {
                        throw std::runtime_error("no uuid / class params key");
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


        void PfAdapterManager::init(const std::string &xmlPath)
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

                            std::string strClass = mChildEle->Attribute("class");

                            auto Itor = mManagement.find(strId);

                            if (Itor != mManagement.end())
                            {
                                strErr.str("");
                                strErr << "[ERROR]CDevManager" << "设备ID(" << strId << ")" << "重复定义";
                                throw std::runtime_error(strErr.str());
                            }

                            bool isLoad = false;

#if defined(Q_OS_WIN)
                            std::string dllPath = "./adapterLib/" + strClass + ".dll";
#else
                            std::string dllPath = "./adapterLib/lib" + strClass + ".so";
#endif

                            LOAD_STEP_LIB libfun = nullptr;

                            QLibrary lib(dllPath.c_str());//加载*****.dll
                            if (lib.load())//判断是否加载成功
                            {
                                libfun = (LOAD_STEP_LIB)lib.resolve("LoadClass");//获取dll的函数,***为动态库中的函数                                i

                                if(libfun != nullptr)
                                {
                                    Adapter  *initObj = libfun();

                                    try
                                    {
                                        initObj->init(mChildEle);
                                        mManagement[strId] = std::shared_ptr<Adapter>(initObj);
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
                                strErr << "[" << dllPath << "] 加载dll失败:" << strClass << "(" + lib.errorString().toStdString() + ")";
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

        bool PfAdapterManager::isExist(const std::string &id)
        {
            bool res = false;

            auto itor = mManagement.find(id);
            if(itor != mManagement.end())
            {
                res = true;
            }

            return res;
        }

        void PfAdapterManager::deleteAll()
        {
            for(auto itor = mManagement.begin(); itor != mManagement.end(); )
            {
                if( "UI" == (itor->first))
                {
                    itor++;
                }
                else
                {
                    (itor->second).reset();

                    mManagement.erase(itor++);
                }
            }
        }

        bool PfAdapterManager::getAdapter(const std::string &id, Adapter **adapter)
        {
            bool res = false;

            auto itor = mManagement.find(id);
            if(itor != mManagement.end())
            {
                *adapter = itor->second.get();
                res = true;
            }

            return res;
        }

        void PfAdapterManager::setAdapter(const std::string &id, Adapter *adapter)
        {
            mManagement[id] = std::shared_ptr<Adapter>(adapter);
        }
    }
}
