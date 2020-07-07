#include "framexk.h"

#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"
#include <QTextCodec>
#include <QByteArray>

namespace Pf
{
    namespace PfIcdWorkBench
    {

        frameXK::frameXK()
        {
        }

        frameXK::~frameXK()
        {
        }

        void frameXK::init(const TiXmlElement *ele)
        {

        }

        std::shared_ptr<frameObj>  frameXK::clone()
        {
            frameXK *obj = new frameXK();

            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void frameXK::resendMsg(byteArray &outValue)
        {

        }

        //通用仿真
        void frameXK::simulation(byteArray &outValue, const std::string &json)
        {
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

            byteArray msg;

            //step2-1 域仿真
            _fillRegion(msg, regionJson);

            outValue.push_back(0xEB);
            outValue.push_back(0x90);

            outValue.push_back(0);
            outValue.push_back(0);

            std::copy(msg.begin(), msg.end(), std::back_inserter(outValue));

            //更新长度
            outValue[2] = msg.size() & 0xFF;
            outValue[3] = msg.size() >> 8;
        }

        void frameXK::_fillRegion(byteArray &outValue, const Json::Value &regionJs)
        {
            if(regionJs.isNull())
                return ;

            outValue.clear();
            int outSize = 0;

            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            dataStorage data;
            int preStartPos = 0;

            //获取表号
            int tableNum = regionJs["table_num"].asInt();
            //通过表号 从数据库中获取参数信息

            //添加表号
            //outValue.push_back(tableNum & 0xFF);
            //outValue.push_back(tableNum >> 8);

            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(tableNum, paramValues);

            //qDebug() << paramValues.toStyledString().c_str();

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
                    //sprintf((char*)&tmpBuf[startPos], "%s", initValue.c_str());
                    memcpy(tmpBuf + startPos, initValue.c_str(), initValue.size());
                    preStartPos = startPos + initValue.size();
                    outSize += initValue.size();
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
                }
            }

            std::copy(tmpBuf, tmpBuf + outSize, std::back_inserter(outValue));
        }

        //自定义仿真
        void frameXK::simulation(byteArray &outValue, const Json::Value &root)
        {
            unsigned char tmpBuf[2048] = {0};
            unsigned int msgSize = 2048;
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            std::ostringstream strErr;
            /*Json::Reader reader;
            Json::Value root;
            if(!reader.parse(json, root))
            {
                strErr.str("");
                strErr << "json格式错误(" << json << ")";

                UT_THROW_EXCEPTION(strErr.str());
            }
*/

            //step1：帧头
            byteSize = 2;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, 0x90EB);
            pos += byteSize;

            //step2：帧长度
            byteSize = 2;
            pos += byteSize;

            //step3：特征字节
            byteSize = 1;
            if(!root["frame_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, root["frame_type"].asInt());
            }
            pos += byteSize;

            //step4：子帧类型

            unsigned char subFrameType = 0;

            byteSize = 1;
            if(!root["sub_frame_type"].isNull())
            {
                subFrameType = root["sub_frame_type"].asInt();
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, root["sub_frame_type"].asInt());
            }
            pos += byteSize;

            if( (0xF1 == subFrameType ) || (0xF6 == subFrameType ))
            {

            }
            else if(0xF2 == subFrameType)   //文件信息
            {
                //文件长度
                byteSize = 4;
                if(!root["file_len"].isNull())
                {
                    data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, root["file_len"].asInt());
                }
                pos += byteSize;

                //MD5(16字节)

                byteSize = 16;

                if(!root["MD5"].isNull())
                {
                    std::string md5 = root["MD5"].asString();

                    QByteArray convert(md5.c_str(), md5.size());

                    int tmpPos = pos;

                    for(int index = 0; index < convert.size(); index++)
                    {
                        tmpBuf[tmpPos++] = convert[index];
                    }
                }

                pos += byteSize;


                //文件名称
                byteSize = 25;

                if(!root["file_name"].isNull())
                {
                    memset(tmpBuf + pos, 0, byteSize);
                    if(root["file_name"].asString().size() < 25)
                        memcpy(tmpBuf + pos, root["file_name"].asString().c_str(), root["file_name"].asString().size());
                    else
                        memcpy(tmpBuf + pos, root["file_name"].asString().c_str(), byteSize);
                }

                pos += byteSize;

            }
            else if(0xF5 == subFrameType)   //文件数据请求
            {
                //序号帧
                byteSize = 4;
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, root["file_num"].asInt());
                pos += byteSize;
            }
            else if(0xF7 == subFrameType)   //数据帧
            {
                //序号帧
                byteSize = 4;
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, root["file_num"].asInt());
                pos += byteSize;

                //数据 <= 1400
                if(!root["data"].isNull())
                {
                    std::string tmp = root["data"].asString();

                    QByteArray convert(tmp.c_str(), tmp.size());

                    int tmpPos = pos;

                    for(int index = 0; index < convert.size(); index++)
                    {
                        tmpBuf[tmpPos++] = convert[index];
                    }

                    pos += convert.size();
                }
            }


            //step4:更新长度
            data.setData(tmpBuf, msgSize, 2, 2, 0, 0, pos - 4);

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));
        }

        bool frameXK::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
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

            unsigned short calLen = u32Size - 4;
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

            int subFrameType = 0;
            int frameType = 0;
            //step3：特征字节
            byteSize = 1;
            frameType = data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            result["frame_type"] = frameType;

            pos += byteSize;

            //step4：子帧类型
            byteSize = 1;
            subFrameType = data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
            result["sub_frame_type"] = subFrameType;
            pos += byteSize;

            if(frameType >= 0x91 && frameType <= 0x9E)
            {
                if( (0xF1 == subFrameType ) || (0xF6 == subFrameType ))
                {

                }
                else if(0xF2 == subFrameType)   //文件信息
                {
                    //文件长度
                    byteSize = 4;
                    result["file_len"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
                    pos += byteSize;
                    //MD5(16字节)

                    byteSize = 16;
                    if(pos + byteSize <= u32Size)
                    {
                        QByteArray convert((const char*)&u8Msg[pos], byteSize);
                        std::string md5 = convert.toStdString();
                        result["MD5"] = md5;
                    }
                    pos += byteSize;

                    //文件名称
                    byteSize = 25;
                    if(pos + byteSize <= u32Size)
                    {
                        result["file_name"] = std::string((const char*)&u8Msg[pos], byteSize);
                    }
                }
                else if(0xF5 == subFrameType)   //文件数据请求
                {
                    //序号帧
                    byteSize = 4;
                    result["file_num"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
                    pos += byteSize;
                }
                else if(0xF7 == subFrameType)   //数据帧
                {
                    //序号帧
                    byteSize = 4;
                    result["file_num"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
                    pos += byteSize;

                    //数据 <= 1400
                    byteSize = u32Size - pos;

                    if(byteSize > 0)
                    {
                        result["data_size"] = byteSize;
                        result["data"] = QByteArray((const char*)&u8Msg[pos], byteSize).toStdString();
                    }
                }
            }
            else if(0x61 == frameType)
            {
                byteSize = 1;
                result["data"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);
                pos += byteSize;
            }

            return !result.isNull();
        }


        extern "C"
        {
            frameObj *LoadClass()
            {
                return new frameXK();
            }
        }
    }
}
