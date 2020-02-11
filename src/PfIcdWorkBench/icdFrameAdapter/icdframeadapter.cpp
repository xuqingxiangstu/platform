#include "icdframeadapter.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../../PfCommon/TinyXml/tinyxml.h"

#include <sstream>
#include <windows.h>


namespace Pf
{
    namespace PfIcdWorkBench
    {

        icdFrameAdapter::icdFrameAdapter()
        {

        }

        icdFrameAdapter::~icdFrameAdapter()
        {

        }

        std::shared_ptr<frameObj> icdFrameAdapter::getFrameObj(const std::string &key)
        {
            bool res = false;

            auto itor = mFrameManagement.find(key);
            if(itor == mFrameManagement.end())
            {
                return nullptr;
            }

            return (itor->second)->clone();
        }

        void icdFrameAdapter::init(const std::string &xmlPath)
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
                            std::string strId = mChildEle->Attribute("key");

                            std::string strClass = mChildEle->Attribute("frame");

                            auto Itor = mFrameManagement.find(strId);

                            if (Itor != mFrameManagement.end())
                            {
                                strErr.str("");
                                strErr << "[ERROR]CDevManager" << "设备ID(" << strId << ")" << "重复定义";
                                UT_THROW_EXCEPTION(strErr.str());
                            }

                            bool isLoad = false;

                            std::string dllPath = "./frameLib/" + strClass + ".dll";

                            HMODULE hd = LoadLibraryW(std::wstring(dllPath.begin(), dllPath.end()).c_str());
                            if(hd != 0)
                            {
                                LOAD_FRAME_LIB libfun = (LOAD_FRAME_LIB)GetProcAddress(hd, "LoadClass");
                                if(libfun != nullptr)
                                {
                                    frameObj  *initObj = libfun();

                                    try
                                    {
                                        initObj->init(mChildEle);
                                        mFrameManagement[strId] = std::shared_ptr<frameObj>(initObj);
                                        isLoad = true;
                                    }
                                    catch(std::runtime_error err)
                                    {
                                        UT_THROW_EXCEPTION(strId + " 初始化失败(" + err.what() + ")");
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
    }
}
