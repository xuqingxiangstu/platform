#ifndef ICDMANAGER_H
#define ICDMANAGER_H

#include <mutex>
#include <memory>

#include <QMap>
#include <QString>

#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

#define FRAME_BE    "BE"
#define FRAME_FE    "FE"
#define FRAME_93    "密码集控装置协议"
#define FRAME_C3    "C3协议"
#define FRAME_MIDDLE    "中间件"
#define FRAME_XK        "信控协议"
#define FRAME_DM        "定位瞄准协议"
#define FRAME_CZXK      "车长显控通讯协议"
#define FRAME_M1553B    "M1553B协议"
#define FRAME_JG        "机柜协议"

using namespace Pf;

/**
 * @brief The icdManager class  帧解析管理，初始化帧解析、获取解析类、帧类型映射
 */

class icdManager
{
public:
    /** 帧类型 **/
    enum frameType{
        Frame_BE = 1,
        Frame_FE,
        Frame_Middle
    };
public:
    static std::shared_ptr<icdManager> getInstance()
    {
        if(mInstance == nullptr)
        {
            std::lock_guard<std::mutex> lk(mMutex);
            if(mInstance == nullptr)
            {
                mInstance = std::make_shared<icdManager>();
            }
        }

        return mInstance;
    }

public:
    icdManager();
    ~icdManager();
public:
    /**
     * @brief getFrameObj   通过帧类型获取帧解析类
     * @param type          帧类型（日志文件中）
     * @return
     */
    std::shared_ptr<PfIcdWorkBench::frameObj> getFrameObj(const int &type);

    /**
     * @brief getFrameObj   通过帧关键字获取帧解析类
     * @param key          关键字（icd.xml配置）
     * @return
     */
    std::shared_ptr<PfIcdWorkBench::frameObj> getFrameObj(const std::string &key);

    /**
     * @brief getMapping    通过帧类型获取映射帧字符串
     * @param type          帧类型
     * @return
     */
    QString getMapping(const int &type);
private:
    std::shared_ptr<Pf::PfIcdWorkBench::icdFrameAdapter> mIcdFrameAdpter; ///<组帧、解帧协议适配对象
    QMap<int, QString> mFrameTypeMapping;   ///< 帧类型映射
private:
    static std::mutex mMutex;
    static std::shared_ptr<icdManager> mInstance;
};

#endif // ICDMANAGER_H
