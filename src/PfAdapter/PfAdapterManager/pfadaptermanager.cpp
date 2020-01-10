#include "pfadaptermanager.h"

#include <sstream>
#include <windows.h>

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

                            std::string dllPath = "./adapterLib/" + strClass + ".dll";

                            HMODULE hd = LoadLibraryW(std::wstring(dllPath.begin(), dllPath.end()).c_str());
                            if(hd != 0)
                            {
                                LOAD_STEP_LIB libfun = (LOAD_STEP_LIB)GetProcAddress(hd, "LoadClass");
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
                                strErr << "[" << dllPath << "] 加载dll失败:" << strClass;
                                UT_THROW_EXCEPTION(strErr.str());
                            }
#if 0
                            RuntimeClass *rtClass = RuntimeClass::LoadByName(strClass); //根据名字找到运行时类指针
                            if (rtClass == NULL)
                            {
                                strErr.str("");
                                strErr << "[ERROR][" << __FILE__ << __LINE__ << __FUNCTION__ << "]动态创建类型失败:" << strClass;
                                throw std::runtime_error(strErr.str());
                            }

                            DynObject *obj = (DynObject*)rtClass->CreateObject();//根据运行类指针创建对象
                            if (obj == NULL)
                            {
                                strErr.str("");
                                strErr << "[ERROR][" << __FILE__ << __LINE__ << __FUNCTION__ << "]动态创建对象失败:" << strClass;
                                throw std::runtime_error(strErr.str());
                            }

                            Adapter *initObj = (Adapter*)obj;
                            try
                            {
                                initObj->init(mChildEle);
                                mManagement[strId] = std::shared_ptr<Adapter>(initObj);
                            }
                            catch(std::runtime_error err)
                            {
                                throw std::runtime_error(strId + " 初始化失败(" + err.what() + ")");
                            }
#endif
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
