#include "zy.h"

#include "../src/PfCommon/tools/ut_error.h"
#include "../../src/PfCommon/jsoncpp/json.h"
#include "../../src/PfCommon/crc/crc.h"

#include "../business/xk/xkbusiness.h"

#include <exception>
#include <sstream>
#include <QDateTime>
#include <QThread>
#include <QDebug>
#include <chrono>

using namespace  Pf::PfIcdWorkBench;

/****************************************************/

void zyWriteFile::save(QString filePath, const QByteArray &msg)
{
    QFile file(filePath);

    file.open(QIODevice::WriteOnly);

    file.write(msg.data(), msg.size());

    file.close();
}



/****************************************************/

zyReadFile::zyReadFile(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        throw std::runtime_error("open file error" + filePath.toStdString());
    }

    mFileValue = file.readAll();
    mFileMaxSize = mFileValue.size();
    mCurMd5 = Pf::PfCommon::Crc::calMd5((const unsigned char*)mFileValue.data(), mFileValue.size());
}

int zyReadFile::getSize()
{
    return mFileMaxSize;
}


std::string zyReadFile::getMd5()
{
    return mCurMd5;
}

int zyReadFile::getPackSize()
{
    int packSize = mFileMaxSize / PACK_MAX_SIZE;

    if(mFileMaxSize % PACK_MAX_SIZE)
        packSize++;

    return packSize;
}

bool zyReadFile::getPackData(int index, std::string &msg, int &msgSize)
{
    bool res = false;

    int packSize = mFileMaxSize / PACK_MAX_SIZE;
    int beyond = mFileMaxSize % PACK_MAX_SIZE;

    if( (index - 1) == packSize)    //最后一包
    {
        if(beyond > 0)
        {
            msg = mFileValue.mid((index - 1) * PACK_MAX_SIZE, beyond).toStdString();
            msgSize = beyond;
            res = true;
        }
    }
    else if((index - 1) > packSize) //无效包
    {
        res = false;
    }
    else    //整包
    {
        msg = mFileValue.mid((index - 1) * PACK_MAX_SIZE, PACK_MAX_SIZE).toStdString();
        msgSize = PACK_MAX_SIZE;
        res = true;
    }

    return res;
}

/****************************************************/

zy::zy(const std::string &uuid, const std::string &node, std::shared_ptr<Pf::PfIcdWorkBench::frameObj> icdObj, Pf::PfAdapter::Adapter *busObj, Pf::PfAdapter::Adapter *uiObj)
{
    std::ostringstream errInfo;

    mRecordUuid = uuid;
    mFrameObj = icdObj;
    mBusAdapter = busObj;
    mUiAdapter = uiObj;

    //获取带操作的诸元类型
    Zy_Index index = (Zy_Index)std::atoi(node.c_str());

    if( index > ZY_MAX_INDEX)
    {
        errInfo << "诸元索引错误";
        UT_THROW_EXCEPTION(errInfo.str());
    }

    //传输类型
    Zy_Trans_Type transType = mZyTransType[index];

    if(Down_Zy == transType)
    {
        down(index, Down_Trigger);
    }
    else if(Up_Zy == transType)
    {
        up(index);
    }
    else if(Down_Up_Zy == transType)
    {
        down_Up(index);
    }
}


void zy::up(Zy_Index index)
{
    std::ostringstream showInfo;
    //获取文件信息
    QString file = mZyFileName.at(index);

    std::string path = mZyFilePath + file.toStdString();

    zyReadFile fileObj(QString::fromStdString(path));    

    showInfo.str("");
    showInfo << "发送文件信息 [" << file.toStdString() << "] size(" << std::dec << fileObj.getSize()<< ")" << ",pack(" << fileObj.getPackSize() << ")";
    toUi(showInfo.str());

    //发送文件信息
    sendFileInfo(mZyFrameType[index], fileObj.getSize(), fileObj.getMd5(), file.toStdString());

    //接收数据请求帧
    const int rcvMaxSize = 2048;
    char rcvBuf[rcvMaxSize] = {0};
    int rcvSize = 0;
    std::string rcvIp;
    unsigned short rcvPort;

    int timeOut = 5000; //5s超时退出
    auto beginTime = std::chrono::high_resolution_clock::now();

    while(1)
    {
        if(mBusAdapter->receiveMsg(rcvBuf, rcvSize, rcvMaxSize, 0))
        {
            Json::Value result;
            mFrameObj->parse((unsigned char *)rcvBuf, rcvSize, result);
            if(!result.isNull())
            {
                int frameType = result["frame_type"].asInt();

                if(frameType >= ZY_STAT && frameType <= ZY_STOP)  //诸元处理
                {
                    int subType = result["sub_frame_type"].asInt();
                    if(mSubFrameType[Sub_File_Request_Index] == subType)//文件请求
                    {
                        int packIndex = result["file_num"].asInt();

                        showInfo.str("");
                        showInfo << "请求第 " << std::dec << packIndex << " 包";
                        toUi(showInfo.str());

                        std::string data;
                        int dataSize = 0;
                        if(fileObj.getPackData(packIndex, data, dataSize))
                           sendFileDataCmd(mZyFrameType[index], packIndex, data, dataSize);
                    }
                    else if(mSubFrameType[Sub_File_Over_Index] == subType)//结束
                    {
                        toUi("文件传输结束");
                        break;
                    }
                }
                else if(0x61 == frameType)//发射进程
                {
                    testFlow(result);
                }
                else if(0xAF == frameType)//心跳
                {
                    heart();
                }
            }        
        }

        QThread::msleep(1);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime= std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);

        if(elapsedTime.count() >= timeOut)   //超时退出
        {
            toUi("超时退出！", false);

            break;
        }
    }
}

void zy::heart()
{
    byteArray msg;
    if(mFrameObj)
    {
        Json::Value info;
        info["frame_type"] = 0xAF;
        info["sub_frame_type"] = 0xAF;

        mFrameObj->simulation(msg, info);
    }
    if(mBusAdapter && (msg.size() > 0))
    {
        mBusAdapter->sendMsg((const char*)&msg.at(0), msg.size());
    }
}

void zy::testFlow(const Json::Value &result)
{
    bool isError = false;

    int subType = result["sub_frame_type"].asInt();
    int data = result["data"].asInt();

    0xAA == data ? isError = false : isError = true;

    std::string name = xkBusiness::getFlowName(subType);

    if(name != "")
        toUi(name, !isError);
}

bool zy::package(QByteArray &srcMsg, QByteArray &curMsg)
{
    bool res = false;
    QByteArray headData;
    headData.append(0xEB);
    headData.append(0x90);

    if(srcMsg.size() < 2)
        return false;

    curMsg.clear();

    int findPos = srcMsg.mid(2, srcMsg.size() - 2).indexOf(headData);
    if(findPos != -1)
    {
        curMsg = srcMsg.mid(0, findPos + 2);
        srcMsg = srcMsg.mid(findPos + 2);
    }
    else
    {
        curMsg = srcMsg;
        srcMsg.clear();
    }

    return true;
}

void zy::down(Zy_Index index, DownType type)
{
    std::ostringstream showInfo;
    int fileSize = 0;
    std::string md5 = "";
    std::string fileName = "";    
    std::ostringstream errInfo;
    int rcvDataSize = 0;

    QByteArray rcvFileContext;

    int pckIndex = 1;

    const int rcvMaxSize = 2048;
    char rcvBuf[rcvMaxSize] = {0};
    int rcvSize = 0;
    std::string rcvIp;
    unsigned short rcvPort;

    showInfo.str("");
    showInfo << "请求下发文件 ";
    toUi(showInfo.str());

    //下发文件请求
    downFileCmd(mZyFrameType[index]);

    rcvDataSize = 0;

    QByteArray curMsg;

    int timeOut = 5000; //5s超时退出
    auto beginTime = std::chrono::high_resolution_clock::now();
    while(1)
    {
        if(mBusAdapter->receiveMsg(rcvBuf, rcvSize, rcvMaxSize, 0))
        {            
            //QByteArray srcMsg(rcvBuf, rcvSize);
            //qDebug() << srcMsg.toHex();

            //TCP出现粘包
            //while(package(srcMsg, curMsg))
            {
                Json::Value result;
                mFrameObj->parse((unsigned char *)rcvBuf, rcvSize, result);
                if(!result.isNull())
                {
                    int frameType = result["frame_type"].asInt();

                    if(frameType >= ZY_STAT && frameType <= ZY_STOP)  //诸元处理
                    {
                        if(mSubFrameType[Sub_File_Info_Index] == result["sub_frame_type"].asInt())//文件信息
                        {
                            if(!result["file_len"].isNull())
                            {
                                fileSize = result["file_len"].asInt();
                            }

                            if(!result["MD5"].isNull())
                            {
                                md5 = result["MD5"].asString();
                            }

                            if(!result["file_name"].isNull())
                            {
                                fileName = "HC";
                                fileName += result["file_name"].asString();
                            }

                            showInfo.str("");
                            showInfo << "文件信息 [" << fileName << "] size(" << std::dec << fileSize << ")";
                            toUi(showInfo.str());

                            pckIndex = 1;
                            rcvDataSize = 0;
                            rcvFileContext.clear();

                            //读取文件信息之后请求帧
                            showInfo.str("");
                            showInfo << "请求第 （ " << std::dec << pckIndex << "）包";
                            toUi(showInfo.str());

                            //文件数据请求指令
                            requestFileDataCmd(mZyFrameType[index], pckIndex++);
                        }
                        else if(mSubFrameType[Sub_File_Data_Index] == result["sub_frame_type"].asInt())//文件数据帧
                        {
                            int packNum = 0;
                            int msgSize = 0;
                            std::string msg;
                            if(!result["file_num"].isNull())
                            {
                                packNum = result["file_num"].asInt();
                            }

                            if(!result["data_size"].isNull())
                            {
                                msgSize = result["data_size"].asInt();
                            }

                            if(!result["data"].isNull())
                            {
                                msg = result["data"].asString();
                            }

                            rcvFileContext.append(msg.c_str(), msgSize);

                            showInfo.str("");
                            showInfo << "接收第 （ " << std::dec << packNum << "）包, size(" << msgSize << ")";
                            toUi(showInfo.str());

                            rcvDataSize += msgSize;

                            if(rcvDataSize >= fileSize)
                            {
                                std::string downFile = mZyFilePath + fileName;

                                zyWriteFile::save(QString::fromStdString(downFile), rcvFileContext);

                                //文件校验
                                std::string calMd5 = Pf::PfCommon::Crc::calMd5((const unsigned char*)rcvFileContext.data(), rcvFileContext.size());

                                if(calMd5 != md5)
                                {
                                    errInfo.str("");
                                    errInfo << "md5 check error src(" << md5 << "), cal(" << calMd5 << ")";
                                    toUi(errInfo.str());
                                }

                                //文件传输结束

                                showInfo.str("");
                                showInfo << "文件传输结束";

                                toUi(showInfo.str());

                                requestFileDataOverCmd(mZyFrameType[index]);

                                break;
                            }
                            else
                            {
                                if(Down_Request == type)
                                {
                                    showInfo.str("");
                                    showInfo << "请求第 （ " << std::dec << pckIndex << "）包";
                                    toUi(showInfo.str());

                                    //文件数据请求指令
                                    requestFileDataCmd(mZyFrameType[index], pckIndex++);
                                }
                            }
                        }
                    }
                    else if(0x61 == frameType)//发射进程
                    {
                        testFlow(result);
                    }
                    else if(0xAF == frameType)//心跳
                    {
                        heart();
                    }
                }
            }
        }

        QThread::usleep(10);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime= std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);

        if(elapsedTime.count() >= timeOut)   //超时退出
        {
            toUi("超时退出！", false);
            break;
        }
    }
}

void zy::down_Up(Zy_Index index)
{
   down(index, Down_Request);
   QThread::msleep(100);
   up(index);
}

void zy::sendFileDataCmd(const unsigned char &zyType,const int &packNum, const std::string &data, const int &dataSize)
{
    Json::Value info;
    info["frame_type"] = zyType;
    info["sub_frame_type"] = mSubFrameType[Sub_File_Data_Index];
    info["file_num"] = packNum;
    info["data_size"] = dataSize;
    info["data"] = data;

    byteArray msg;
    mFrameObj->simulation(msg, info);

    if(mBusAdapter && msg.size() > 0)
    {
        mBusAdapter->sendMsg((const char*)&msg.at(0), msg.size());
    }
}

bool zy::rcvDataCmd(int &packNum, std::string &msg, int &msgSize)
{
    const int rcvMaxSize = 2048;
    char rcvBuf[rcvMaxSize] = {0};
    int rcvSize = 0;
    std::string rcvIp;
    unsigned short rcvPort;
    std::ostringstream errInfo;
    bool res = false;

    if(mBusAdapter)
    {
        if(!mBusAdapter->receiveMsg(rcvBuf, rcvSize, rcvMaxSize, TIME_OUT))
        {
            errInfo << "请求数据帧失败";
            UT_SHOW(errInfo.str());
            res = false;
        }
        else
        {
            Json::Value result;
            mFrameObj->parse((unsigned char *)rcvBuf, rcvSize, result);
            if(!result.isNull())
            {
                if(mSubFrameType[Sub_File_Data_Index] == result["sub_frame_type"].asInt())//文件数据帧
                {
                    if(!result["file_num"].isNull())
                    {
                        packNum = result["file_num"].asInt();
                    }

                    if(!result["data_size"].isNull())
                    {
                        msgSize = result["data_size"].asInt();
                    }

                    if(!result["data"].isNull())
                    {
                        msg = result["data"].asString();
                    }

                    res = true;
                }
            }
        }
    }

    return res;
}

bool zy::sendFileInfo(const unsigned char &zyType, const int &fileSize, const std::string &md5, const std::string &fileName)
{
    Json::Value info;
    info["frame_type"] = zyType;
    info["sub_frame_type"] = mSubFrameType[Sub_File_Info_Index];
    info["file_len"] = fileSize;
    info["MD5"] = md5;
    info["file_name"] = fileName;

    byteArray msg;    
    mFrameObj->simulation(msg, info);

    if(mBusAdapter && msg.size() > 0)
    {
        mBusAdapter->sendMsg((const char*)&msg.at(0), msg.size());
    }
}


void zy::downFileCmd(const unsigned char &zyType)
{
    byteArray msg;

    Json::Value info;
    info["frame_type"] = zyType;
    info["sub_frame_type"] = mSubFrameType[Sub_Down_File_Index];

    mFrameObj->simulation(msg, info);

    if(mBusAdapter && msg.size() > 0)
    {
        mBusAdapter->sendMsg((const char*)&msg.at(0), msg.size());
    }
}

void zy::requestFileDataOverCmd(const unsigned char &zyType)
{
    byteArray msg;

    Json::Value info;
    info["frame_type"] = zyType;
    info["sub_frame_type"] = mSubFrameType[Sub_File_Over_Index];

    mFrameObj->simulation(msg, info);

    if(mBusAdapter && msg.size() > 0)
    {
        mBusAdapter->sendMsg((const char*)&msg.at(0), msg.size());
    }
}

void zy::requestFileDataCmd(const unsigned char &zyType, const int &packIndex)
{
    byteArray msg;

    Json::Value info;
    info["frame_type"] = zyType;
    info["sub_frame_type"] = mSubFrameType[Sub_File_Request_Index];
    info["file_num"] = packIndex;

    mFrameObj->simulation(msg, info);

    if(mBusAdapter && msg.size() > 0)
    {
        mBusAdapter->sendMsg((const char*)&msg.at(0), msg.size());
    }
}

void zy::toUi(const std::string &msg, bool state)
{
    Json::Value js;
    js["msgType"] = "showMsg";

    Json::Value tmp;
    tmp["record_uuid"] = mRecordUuid;

    if(state)
        tmp["status"] = "NORMAL";
    else
        tmp["status"] = "ERROR";

    tmp["show"] = "[" + QDateTime::currentDateTime().toString("hh:mm.ss.zzz").toStdString() + "]" + msg;

    js["msg"] = tmp;

    if(mUiAdapter)
        mUiAdapter->sendMsg(js.toStyledString().c_str(), js.toStyledString().size());
}

