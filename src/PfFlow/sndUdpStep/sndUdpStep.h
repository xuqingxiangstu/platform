#ifndef SNDUDPSTEP_H
#define SNDUDPSTEP_H

#include "sndUdpStep_global.h"

#include "../runObj/runobj.h"
/** 版本号 **/
#define VERSION "1.0.0"


class SNDUDPSTEPSHARED_EXPORT sndUdpStep : public runObj
{
public:
    sndUdpStep();
    ~sndUdpStep();
public:
    void init(const TiXmlElement *ele) override;
    std::string getClassName() override;
    bool exec() override;
    std::string getVarValue(std::string) override;
    bool getResult() override;
    std::string outText() override;
    void setAutoRunObj(runObj *obj) override;
    void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter) override;
    void setIcdframeadapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter) override{mIcdAdapter = icdAdapter;}
    std::string version() override {return VERSION;}
private:
    std::string&  replaceAll(std::string&   str,const   std::string&   old_value,const   std::string&   new_value);
    runObj *mRunObj;    ///< 运行类
    bool mResult; ///< 测试结果
    Pf::PfAdapter::PfAdapterManager *mAdapter; ///< 适配器管理
    Pf::PfIcdWorkBench::icdFrameAdapter *mIcdAdapter; ///< ICD帧管理
    std::string strOutText; ///< 输出信息
    std::string strProtocolId;
    std::string strDevId;
    std::string strJson;
};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    SNDUDPSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // SNDUDPSTEP_H
    
    
