#include "icdframeadapter.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../../PfCommon/TinyXml/tinyxml.h"

#include <sstream>

#include <QLibrary>
#include <QDebug>


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

#if defined(Q_OS_WIN)
                            std::string dllPath = "./frameLib/" + strClass + ".dll";
#else
                            std::string dllPath = "./frameLib/lib" + strClass + ".so";
#endif

                            LOAD_FRAME_LIB libfun = nullptr;

                            QLibrary lib(dllPath.c_str());//加载*****.dll
                            if (lib.load())//判断是否加载成功
                            {
                                libfun = (LOAD_FRAME_LIB)lib.resolve("LoadClass");//获取dll的函数,***为动态库中的函数                                i

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
    }
}
