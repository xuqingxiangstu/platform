#ifndef ZY_H
#define ZY_H

#include <string>
#include <QStringList>
#include <QFile>

#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"
#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"

//#define ZY_MSG  "60000"

#define PACK_MAX_SIZE  1400

#define TIME_OUT    -1

class zyWriteFile
{
public:
    static void save(QString filePath, const QByteArray &msg);
};

class zyReadFile
{
public:
    zyReadFile(QString filePath);
public:
    /**
     * @brief getSize   获取总长度
     * @return  长度
     */
    int getSize();

    /**
     * @brief getMd5    获取MD5校验
     * @return 校验
     */
    std::string getMd5();

    /**
     * @brief getPackData   获取包数据
     * @param index     包号
     * @param msg       数据
     * @param msgSize   数据大小
     * @return
     */
    bool getPackData(int index, std::string &msg, int &msgSize);

    /**
     * @brief getPackSize   获取总包大小
     * @return 包大小
     */
    int getPackSize();
private:

    QByteArray mFileValue;
    int mFileMaxSize;
    std::string mCurMd5;
};

class zy
{
private:
    /** 子帧类型索引 **/
    enum Zy_Sub_Frame_Index
    {
        Sub_Down_File_Index,    ///< 下发文件
        Sub_File_Info_Index,    ///< 文件信息
        Sub_File_Request_Index, ///< 文件请求
        Sub_File_Over_Index,    ///< 文件结束
        Sub_File_Data_Index,    ///< 文件数据帧
        Sub_Max_Index
    };

    /** 子帧类型 **/
    const unsigned char mSubFrameType[Sub_Max_Index] = {
        0xF1,
        0xF2,
        0xF5,
        0xF6,
        0xF7,
    };

    /** 诸元传输类型 **/
    enum Zy_Trans_Type{
        Null_Zy,
        Down_Zy,    ///< 下发（请求一次，剩余服务器主动下发）
        Down_Up_Zy, ///< 下发后回传
        Up_Zy    ///< 回传
    };

    /** 诸元索引 **/
    enum Zy_Index{
        ZY_MSG_NULL_INDEX,
        ZY_MSG_CKT_INDEX,   //下发参考图
        ZY_MSG_DW_INDEX,    //点位诸元
        ZY_MSG_DD_INDEX,    //弹道诸元
        ZY_MSG_MZ_INDEX,    //瞄准参数诸元
        ZY_MSG_BDLD_INDEX,  //被动雷达库
        ZY_MSG_YK_INDEX,    //引控诸元
        ZY_MSG_ZXCK_INDEX,  //战训测控诸元
        ZY_MSG_DJ_INDEX,    //回传单机诸元
        ZY_MSG_TZ_INDEX,    //回传特征参考图诸元
        ZY_MSG_XG_INDEX,    //回传星光诸元
        ZY_MSG_HCLD_INDEX,    //回传被动雷达
        ZY_MAX_INDEX
    };

    /** 诸元文件名 **/
    const QStringList mZyFileName ={
        "",
        "CKTZY.ZY",
        "HCDWZY.ZY",
        "HCDDZY.ZY",
        "HCMZCS.ZY",
        "HCBDLDK.ZY",
        "HCZHYKZY.ZY",
        "HCZXCKZY.ZY",
        "HCDJZY.ZY",
        "HCCKTZY.ZY",
        "HCXGZY.ZY",
        "HCBDLDK.ZY"
    };

    /** 诸元类型 **/
    const unsigned char mZyFrameType[ZY_MAX_INDEX] = {
        0x0,
        0x91,
        0x92,
        0x93,
        0x94,
        0x9B,
        0x9E,
        0x9C,
        0x97,
        0x98,
        0x99,
        0x9B
    };

    const Zy_Trans_Type mZyTransType[ZY_MAX_INDEX] = {
        Null_Zy,
        Down_Zy,
        Down_Up_Zy,
        Down_Up_Zy,
        Down_Up_Zy,
        Down_Up_Zy,
        Down_Up_Zy,
        Down_Up_Zy,
        Up_Zy,
        Up_Zy,
        Up_Zy,
        Up_Zy
    };    

    enum DownType{
        Down_Trigger,
        Down_Request
    };
public:
    zy(const std::string &uuid, const std::string &node, std::shared_ptr<Pf::PfIcdWorkBench::frameObj> icdObj, Pf::PfAdapter::Adapter *busObj, Pf::PfAdapter::Adapter *uiObj);
private:
    void up(Zy_Index index);
    void down(Zy_Index index, DownType type);
    void down_Up(Zy_Index index);

    /**
     * @brief downFileCmd   下发文件指令
     * @param zyType    诸元类型
     */
    void downFileCmd(const unsigned char &zyType);

    /**
     * @brief requestFileDataCmd   文件数据请求帧
     * @param zyType    诸元类型
     */
    void requestFileDataCmd(const unsigned char &zyType, const int &packIndex);


    /**
     * @brief sendFileInfo   发送文件信息
     * @param fileSize      文件大小
     * @param md5           MD5校验
     * @param fileName      文件名
     */
    bool sendFileInfo(const unsigned char &zyType, const int &fileSize, const std::string &md5, const std::string &fileName);

    /**
     * @brief sendFileDataCmd   发送文件数据
     * @param packNum   包号
     * @param msg       内容
     * @param msgSize   消息大小
     */
    void sendFileDataCmd(const unsigned char &zyType,const int &packNum, const std::string &data, const int &dataSize);

    /**
     * @brief rcvDataCmd    接收数据帧指令
     * @param packNum       包号
     * @param msg           内容
     * @return
     */
    bool rcvDataCmd(int &packNum, std::string &msg, int &msgSize);

    /**
     * @brief requestFileDataCmd   文件数据结束
     * @param zyType    诸元类型
     */
    void requestFileDataOverCmd(const unsigned char &zyType);

    void toUi(const std::string &msg, bool state = true);

    void testFlow(const Json::Value &result);
    void heart();
    bool package(QByteArray &srcMsg, QByteArray &curMsg);
private:
    const std::string mZyFilePath = "./zyFile/";
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mFrameObj;
    Pf::PfAdapter::Adapter *mBusAdapter;
    Pf::PfAdapter::Adapter *mUiAdapter;
    std::string mRecordUuid;
};

#endif // ZY_H
