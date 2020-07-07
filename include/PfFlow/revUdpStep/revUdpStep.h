#ifndef REVUDPSTEP_H
#define REVUDPSTEP_H

#include "revUdpStep_global.h"

#include "../runObj/runobj.h"
/** 版本号 **/
#define VERSION "1.0.0"

#include<vector>
class REVUDPSTEPSHARED_EXPORT revUdpStep : public runObj
{
public:
    revUdpStep();
    ~revUdpStep();
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
    runObj *mRunObj;    ///< 运行类
    bool mResult; ///< 测试结果
    Pf::PfAdapter::PfAdapterManager *mAdapter; ///< 适配器管理
    Pf::PfIcdWorkBench::icdFrameAdapter *mIcdAdapter; ///< ICD帧管理
    std::string strOutText; ///< 输出信息
private:
    /**
     * @brief getJsonParamValue
     * 根据参数名获取Json语句的参数
     * @param strParamName
     * @return
     */
    std::string getJsonParamValue(std::string strParamName);
    std::string strJson; ///源数据的Json语句
    std::string strDevId;
    std::string strProtocolId;
    std::string strUploadDataId;
    std::vector<std::string> vecQuoteVar;
    unsigned short usTableNumber;
    unsigned short usCodeNumber;
    bool           mRevResult;//接收到数据赋值为true否则为false

};

extern "C"
{
    /**
    * @brief 加载类
    * @return 类指针
    */
    REVUDPSTEPSHARED_EXPORT runObj *LoadClass();
}

#endif // REVUDPSTEP_H
    
    
