#include "specialframe93.h"

#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"
#include "../frameNumber/framenumber.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {

        specialFrame93::specialFrame93()
        {
        }

        specialFrame93::~specialFrame93()
        {
        }

        void specialFrame93::init(const TiXmlElement *ele)
        {

        }

        std::shared_ptr<frameObj>  specialFrame93::clone()
        {
            specialFrame93 *obj = new specialFrame93();           


            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void specialFrame93::simulation(byteArray &outValue, const Json::Value &json)
        {

        }

        void specialFrame93::simulation(byteArray &outValue, const std::string &json)
        {
            std::ostringstream strErr;

            outValue.clear();

            //step1获取region信息
            //json反序列化
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


            //填充数据域
            std::copy(msg.begin(), msg.end(), std::back_inserter(outValue));

            if(msg.size() <= 0)
                return ;

            //更新计数
            unsigned int src = outValue[0];
            unsigned int dst = 0;

            int cmdCnt = frameNumberManager::getInstance()->getFrameNumber(mCurUuid, getFrameName(), src, dst) & 0xFF;

            outValue[1] = cmdCnt;

             //填充CRC

            unsigned short crc = PfCommon::Crc::calCrc16(&outValue.at(0), outValue.size());

            outValue.push_back(crc & 0xFF);
            outValue.push_back(crc >> 8);
        }
        void specialFrame93::_fillRegion(byteArray &outValue, const Json::Value &regionJs)
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

            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues((unsigned int)tableNum, paramValues);

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
                    //modify xqx 20200423 当为字符串时，如果设置大小则按照设置填充，否则按照字符串大小进行填充
                    if(0 == byteSize)//按照字符串填充
                    {
                        memcpy(tmpBuf + startPos,  initValue.c_str(), initValue.size());
                        preStartPos = startPos + initValue.size();
                        outSize += initValue.size();
                    }
                    else//按大小填充
                    {
                        //先清空，再填充
                        memset(tmpBuf + startPos, 0, byteSize);
                        int cpySize = 0;
                        if(byteSize > initValue.size())
                        {
                            cpySize = initValue.size();
                        }
                        else
                        {
                            cpySize = byteSize;
                        }
                        memcpy(tmpBuf + startPos,  initValue.c_str(), cpySize);
                        preStartPos = startPos + byteSize;
                        outSize += byteSize;
                    }
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

        bool specialFrame93::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
        {
            if(u32Size <= 0)
                return "";
            dataStorage data;
            std::ostringstream strErr;

            if(u32Size <= 0)
                return false;

            //消息标识
            unsigned int frameType = 0;
            frameType = u8Msg[0];

            result["frame_type"] = frameType;

            if(0x93 == frameType)//93帧由 消息标识+子类型组成
            {
                if(u32Size > 2)
                {
                    //子帧标识
                    unsigned char subFrameType = u8Msg[2];

                    frameType = (u8Msg[0] << 8) | u8Msg[2];
                }
            }

            // 根据帧类型查找数据库进行解析

            Json::Value regionJs;

            _parseRegion(frameType, u8Msg, u32Size, regionJs);

            result["region"] = regionJs;

            return !result.isNull();
        }

        bool specialFrame93::getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json)
        {
            return false;
        }

        void specialFrame93::_parseRegion(const unsigned int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
        {
            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(tableNum, paramValues);

            dataStorage data;
            int preValue = 0;
            int preStartPos = 0;

            regionValue["table_num"] = tableNum;

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
                    //modify xqx 2020-6-28 09:28:17 字符串长度大于总长度时按照最小获取（字符串异常），软件不至于崩溃

                    preValue > (u32Size - startPos) ? preValue = u32Size - startPos : preValue;

                    //end

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
                return new specialFrame93();
            }
        }
    }
}

