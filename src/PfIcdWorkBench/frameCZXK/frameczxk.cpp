#include "frameczxk.h"


#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"
#include "../frameNumber/framenumber.h"

#include <QTextCodec>
#include <QByteArray>
#include <QDateTime>


namespace Pf
{
    namespace PfIcdWorkBench
    {

        frameCZXK::frameCZXK()
        {
        }

        frameCZXK::~frameCZXK()
        {
        }

        void frameCZXK::init(const TiXmlElement *ele)
        {

        }

        std::shared_ptr<frameObj>  frameCZXK::clone()
        {
            frameCZXK *obj = new frameCZXK();

            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void frameCZXK::resendMsg(byteArray &outValue)
        {

        }

        //通用仿真
        void frameCZXK::simulation(byteArray &outValue, const std::string &json)
        {
            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int pos = 0;
            int byteSize = 0;

            dataStorage data;

            std::ostringstream strErr;
            Json::Reader reader;
            Json::Value root;
            if(!reader.parse(json, root))
            {
                strErr.str("");
                strErr << "json格式错误(" << json << ")";

                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取域信息
            Json::Value regionJson = root["region"];
            Json::Value headJson = root["head"];

            byteArray msg;

            //step2-1 域仿真
            _fillRegion(msg, regionJson);

            //帧头
            byteSize = 2;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, 0x90EB);
            pos += byteSize;

            //帧长
            byteSize = 2;
            pos += byteSize;

            //帧类型
            QString tmpValue = regionJson["table_num"].asString().c_str();

            bool isOk;
            unsigned int frameType = tmpValue.toUInt(&isOk, 10);

            byteSize = 4;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, frameType);
            pos += byteSize;

            //重传次数
            byteSize = 1;
            pos += byteSize;

            //确认标记
            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, 0);
            pos += byteSize;


            //step5：源节点填充
            unsigned char src = headJson["head_src_node"].asInt() & 0xFF;

            //step5：目标节点填充
            unsigned char dst = headJson["head_dst_node"].asInt() & 0xFF;


            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, src);
            pos += byteSize;

            //新宿
            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, dst);
            pos += byteSize;

            //时间
            QDateTime current = QDateTime::currentDateTime();

            unsigned int time = current.time().hour() * 3600000 + current.time().minute() * 60000 + current.time().second() * 1000 + current.time().msec();

            byteSize = 4;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, time);
            pos += byteSize;

            //帧计数
            int cmdCnt = frameNumberManager::getInstance()->getFrameNumber(mCurUuid, getFrameName(), src, dst);

            byteSize = 4;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, cmdCnt);
            pos += byteSize;

            //校验和
            byteSize = 1;
            pos += byteSize;

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));

            std::copy(msg.begin(), msg.end(), std::back_inserter(outValue));

            //更新长度
            outValue[2] = outValue.size() & 0xFF;
            outValue[3] = outValue.size() >> 8;

            //校验和
            outValue[20] = Pf::PfCommon::Crc::calSum((const unsigned char*)&outValue.at(0), outValue.size());
        }

        void frameCZXK::_fillRegion(byteArray &outValue, const Json::Value &regionJs)
        {
            if(regionJs.isNull())
                return ;

            outValue.clear();
            int outSize = 0;

            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            dataStorage data;
            int preStartPos = 0;
            int strLenPos = 0;
            int strLenByteSize = 0;
            int strLenValue = 0;

            //从数据库中获取参数信息

            unsigned int tableNum = regionJs["table_num"].asUInt();

            Json::Value paramValues;
            paramsTable::getInstance()->getValues(tableNum, paramValues);

            for(int index = 0; index < paramValues.size(); index++)
            {
                int coding = paramValues[index][PARAM_TABLE_CODING_NUM].asInt();
                int startPos = paramValues[index][PARAM_TABLE_PARAM_BYTE_START_POS].asInt();
                int byteSize = paramValues[index][PARAM_TABLE_PARAM_BYTE_SIZE].asInt();
                int bitPos = paramValues[index][PARAM_TABLE_PARAM_BIT_START_POS].asInt();
                int bitSize = paramValues[index][PARAM_TABLE_PARAM_BIT_SIZE].asInt();
                std::string bigSmall = paramValues[index][PARAM_TABLE_L_B_ENDIAN].asString();
                std::string dataType = paramValues[index][PARAM_TABLE_DATA_TYPE].asString();
                std::string initValue = paramValues[index][PARAM_TABLE_INIT_VALUE].asString();

                if(startPos == -1)
                    startPos = preStartPos;

                //查找域中是否赋值
                for(auto v : regionJs["data"])
                {
                    if(v["coding"] == coding)
                    {
                        initValue = v["value"].asString();
                        break;
                    }
                }

                if( dataType == ieee32Type)
                {
                    data.setData(tmpBuf, msgSize, startPos, byteSize, bitPos, bitSize, (float)std::atof(initValue.c_str()), bigSmall);
                    preStartPos = startPos + byteSize;
                    outSize += byteSize;
                }
                else if( dataType == ieee64Type)
                {
                    data.setData(tmpBuf, msgSize, startPos, byteSize, bitPos, bitSize, (double)std::atof(initValue.c_str()), bigSmall);
                    preStartPos = startPos + byteSize;
                    outSize += byteSize;
                }
                else if(ncharType == dataType)
                {

                    //modify xqx 2020-6-16 09:52:18 UTF8转GBK编码
                    QString utf8Str = QString::fromStdString(initValue);
                    initValue = QTextCodec::codecForName("gb2312")->fromUnicode(utf8Str).data();
                    //end

                    //sprintf((char*)&tmpBuf[startPos], "%s", initValue.c_str());
                    memcpy(tmpBuf + startPos, initValue.c_str(), initValue.size());
                    preStartPos = startPos + initValue.size();
                    outSize += initValue.size();

                    //modify xqx 20200420 更新字符串长度
                    data.setData(tmpBuf, msgSize, strLenPos, strLenByteSize, 0, 0, (int)initValue.size());
                }
                else if(nRawType == dataType)   //十六进制原始数据
                {
                    QByteArray msg = QByteArray::fromHex(initValue.c_str());

                    int maxSize = 0;
                    if(0 == byteSize)
                    {
                        maxSize = msg.size();
                    }
                    else
                    {
                        maxSize = byteSize;
                    }

                    int pos = startPos;
                    for(int index = 0; index < maxSize; index++)
                    {
                        if(index < msg.size())
                            data.setData(tmpBuf, msgSize, pos, 1, 0, 0, msg.at(index));
                        else
                            data.setData(tmpBuf, msgSize, pos, 1, 0, 0, 0);

                        pos += 1;
                    }

                    preStartPos = startPos + maxSize;
                    outSize += maxSize;
                }
                else
                {
                    data.setData(tmpBuf, msgSize, startPos, byteSize, bitPos, bitSize, std::atoi(initValue.c_str()), bigSmall);
                    preStartPos = startPos + byteSize;
                    outSize += byteSize;                    
                    strLenPos = startPos;
                    strLenByteSize = byteSize;
                }
            }

            std::copy(tmpBuf, tmpBuf + outSize, std::back_inserter(outValue));
        }

        //自定义仿真
        void frameCZXK::simulation(byteArray &outValue, const Json::Value &json)
        {
            unsigned char tmpBuf[2048] = {0};
            unsigned int msgSize = 2048;
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            std::ostringstream strErr;

            //帧头
            byteSize = 2;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, 0x90EB);
            pos += byteSize;

            //帧长
            byteSize = 2;
            pos += byteSize;

            //帧类型
            unsigned int frameType = json["frame_type"].asInt();

            byteSize = 4;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, frameType);
            pos += byteSize;

            //重传次数
            byteSize = 1;
            pos += byteSize;

            //确认标记
            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, 0);
            pos += byteSize;

            //信源
            //step5：源节点填充
            unsigned char src = json["src_node"].asInt() & 0xFF;

            //step5：目标节点填充
            unsigned char dst = json["dst_node"].asInt() & 0xFF;

            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, src);
            pos += byteSize;

            //新宿
            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, dst);
            pos += byteSize;

            //时间
            QDateTime current = QDateTime::currentDateTime();

            unsigned int time = current.time().hour() * 3600000 + current.time().minute() * 60000 + current.time().second() * 1000 + current.time().msec();

            byteSize = 4;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, time);
            pos += byteSize;

            //帧计数
            auto findItor = mProtocolCnt.find(std::make_pair(src, dst));
            if(findItor == mProtocolCnt.end())
            {
                mProtocolCnt[std::make_pair(src, dst)] = 0;
            }

            byteSize = 4;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, mProtocolCnt[std::make_pair(src, dst)]);
            pos += byteSize;

            mProtocolCnt[std::make_pair(src, dst)] = mProtocolCnt[std::make_pair(src, dst)] + 1;

            //校验和
            byteSize = 1;
            pos += byteSize;

            //消息内容

            //帧类型
            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, json["code"].asInt());
            pos += byteSize;

            //消息
            Json::Value array = json["msg"];
            for(int index = 0; index < array.size(); index++)
            {
                byteSize = 1;
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, array[index].asInt());
                pos += byteSize;
            }


            //更新长度
            data.setData(tmpBuf, msgSize, 2, 2, 0, 0, pos - 4);

            //校验和
            tmpBuf[20] = Pf::PfCommon::Crc::calSum((const unsigned char*)tmpBuf, pos);

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));
        }

        bool frameCZXK::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
        {
            if(u32Size < 2)
                return false;

            dataStorage data;
            std::ostringstream strErr;
            Json::Value headJs;
            int pos = 0;
            int byteSize = 0;

            //step1：帧头校验

            byteSize = 2;
            unsigned short head = data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);

            if(0x90EB != head)
            {
                strErr.str("");
                strErr << "帧同步码校验错误, 接收0x" << std::hex << head << " 实际0xEB90";
                UT_THROW_EXCEPTION(strErr.str());
            }

            pos += byteSize;

            //step3：数据长度校验
            byteSize = 2;
            unsigned short getLen = data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            pos += byteSize;

            unsigned short calLen = u32Size;
            if(getLen != calLen)
            {
                strErr.str("");
                strErr << "数据长度(" << std::dec << u32Size << ") "<< "校验错误, 接收0x" << std::dec << getLen << " 实际计算0x" << calLen;
#ifndef QT_NO_DEBUG
                strErr << std::hex << " ";
                for(int index = 0; index < u32Size; index++)
                {
                    strErr << (int)u8Msg[index] << " ";
                }
#endif
                UT_THROW_EXCEPTION(strErr.str());
            }

            //帧类型
            byteSize = 4;
            unsigned int frameType = data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            result["frame_type"] = frameType;
            pos += byteSize;

            //重传次数
            byteSize = 1;
            result["resend_cnt"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            pos += byteSize;

            //确认标记
            byteSize = 1;
            result["is_ask"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            pos += byteSize;

            //源节点表号
            byteSize = 1;
            result["src_num"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            pos += byteSize;

            //目标节点表号
            byteSize = 1;
            result["dst_num"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            pos += byteSize;

            //时间
            byteSize = 4;
            result["time"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            pos += byteSize;

            //帧计数
            byteSize = 4;
            result["count"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            pos += byteSize;


            //帧校验
            byteSize = 1;
            pos += byteSize;

            //特征吗
            byteSize = 1;
            result["cmd_code"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);

            // 根据帧类型查找数据库进行解析

            Json::Value regionJs;

            _parseRegion(frameType, &u8Msg[pos], u32Size - pos, regionJs);

            result["region"] = regionJs;

            return !result.isNull();
        }

        void frameCZXK::_parseRegion(const unsigned int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
        {
            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(tableNum, paramValues);

            dataStorage data;
            int preValue = 0;
            int preStartPos = 0;

            regionValue["table_num"] = tableNum;

            //TODO:表号

            Json::Value dataJs;

            for(int index = 0; index < paramValues.size(); index++)
            {
                Json::Value tmpValue;
                int coding = paramValues[index][PARAM_TABLE_CODING_NUM].asInt();
                int startPos = paramValues[index][PARAM_TABLE_PARAM_BYTE_START_POS].asInt();
                int byteSize = paramValues[index][PARAM_TABLE_PARAM_BYTE_SIZE].asInt();
                int bitPos = paramValues[index][PARAM_TABLE_PARAM_BIT_START_POS].asInt();
                int bitSize = paramValues[index][PARAM_TABLE_PARAM_BIT_SIZE].asInt();
                std::string bigSmall = paramValues[index][PARAM_TABLE_L_B_ENDIAN].asString();
                std::string dataType = paramValues[index][PARAM_TABLE_DATA_TYPE].asString();
                std::string initValue = paramValues[index][PARAM_TABLE_INIT_VALUE].asString();

                tmpValue["coding"] = coding;

                if(startPos == -1)
                    startPos = preStartPos;

                if(ncharType == dataType)
                {
                    std::string calResult = std::string((const char*)&u8Msg[startPos], preValue);

                    tmpValue["value"] = calResult;

                    preStartPos = startPos + preValue;
                }
                else
                {
                    unDataConvert pValue = data.getAutoData(u8Msg, u32Size, startPos, byteSize, bitPos, bitSize, bigSmall);

                    preValue = pValue.i32Value;
                    preStartPos = startPos + byteSize;

                    if( (dataType == ieee32Type))
                    {
                        tmpValue["value"] = pValue.f32Value;
                    }
                    else if( (dataType == ieee64Type))
                    {
                        tmpValue["value"] = pValue.d64Value;
                    }
                    else
                    {
                        tmpValue["value"] = pValue.i32Value;
                    }
                }
                if(!tmpValue.isNull())
                    dataJs.append(tmpValue);
            }

            if(!dataJs.isNull())
                regionValue["data"] = dataJs;
        }


        extern "C"
        {
            frameObj *LoadClass()
            {
                return new frameCZXK();
            }
        }
    }
}
