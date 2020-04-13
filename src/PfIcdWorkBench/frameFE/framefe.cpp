#include "framefe.h"

#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"


namespace Pf
{
    namespace PfIcdWorkBench
    {

        frameFE::frameFE()
        {
        }

        frameFE::~frameFE()
        {
        }

        void frameFE::init(const TiXmlElement *ele)
        {

        }

        std::shared_ptr<frameObj>  frameFE::clone()
        {
            frameFE *obj = new frameFE();



            for(auto itor = mProtocolCnt.begin() ; itor != mProtocolCnt.end(); itor++)
            {
                obj->mProtocolCnt[itor->first] = (itor->second);
            }


            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void frameFE::resendMsg(byteArray &outValue)
        {
            if(outValue.size() >= 15)
                outValue.at(15) = outValue.at(15) + 1;

            //更新CRC

            unsigned short crc = PfCommon::Crc::calCrc16(&outValue.at(3), outValue.size() - 3);
            outValue.at(1) = crc >> 8;
            outValue.at(2) = crc & 0xFF;
        }

        void frameFE::simulation(byteArray &outValue, const std::string &json)
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
            Json::Value headJson = root["head"];

            byteArray msg;

            //step2-1 域仿真
            _fillRegion(msg, regionJson);

            //step3填充帧头
            outValue.push_back(mHeadCode);

            //step4：填充校验码（先站位，后更新）
            outValue.push_back(0);
            outValue.push_back(0);

            //step5：填充数据长度（先站位，后更新）
            outValue.push_back(0);
            outValue.push_back(0);

            //step5：源节点填充
            int srcNode = headJson["head_src_node"].asInt() & 0xFF;
            outValue.push_back(srcNode & 0xFF);

            //step5：目标节点填充
            int dstNode = headJson["head_dst_node"].asInt() & 0xFF;
            outValue.push_back(dstNode & 0xFF);

            //step6：数据种类类型
            int dataTYpe = headJson["data_type"].asInt();
            outValue.push_back(dataTYpe >> 24);
            outValue.push_back(dataTYpe >> 16);
            outValue.push_back(dataTYpe >> 8);
            outValue.push_back(dataTYpe);

            //step7：预留字节
            outValue.push_back(0);

            //step8：确认标志
            int ack = 0;
            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(regionJson["table_num"].asInt(), paramValues);
            if(paramValues.size() > 0)
            {
                std::string isAck = paramValues[0][PARAM_TABLE_IS_ACK].asString();
                if(IS_ACK_YES == isAck)
                    ack = 0x1;
                else if(IS_ACK_NO == isAck)
                    ack = 0x0;
            }

            outValue.push_back(ack);

            //int retrans = root["resendCnt"].asInt() & 0xFF;
            int retrans = 0;

            //step9: 填充命令计数,

            auto findItor = mProtocolCnt.find(std::make_pair(srcNode, dstNode));
            if(findItor == mProtocolCnt.end())
            {
                mProtocolCnt[std::make_pair(srcNode, dstNode)] = 0;
            }

            unsigned short cmdCnt = mProtocolCnt[std::make_pair(srcNode, dstNode)];

            outValue.push_back(cmdCnt >> 8);
            outValue.push_back(cmdCnt & 0xFF);

            mProtocolCnt[std::make_pair(srcNode, dstNode)] = mProtocolCnt[std::make_pair(srcNode, dstNode)] + 1;

            //step9：重传次数

            outValue.push_back(retrans);

            //step10: 填充数据域
            std::copy(msg.begin(), msg.end(), std::back_inserter(outValue));

            //step11：更新长度
            int len = outValue.size() - 5;
            outValue.at(3) = len >> 8;
            outValue.at(4) = len & 0xFF;

            //step12：更新CRC
            unsigned short crc = PfCommon::Crc::calCrc16(&outValue.at(3), outValue.size() - 3);
            outValue.at(1) = crc >> 8;
            outValue.at(2) = crc & 0xFF;
        }
        void frameFE::_fillRegion(byteArray &outValue, const Json::Value &regionJs)
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
            outValue.push_back(tableNum & 0xFF);
            outValue.push_back(tableNum >> 8);

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
                std::string initValue = paramValues[index][PARAM_TABLE_PARAM_BIT_SIZE].asString();

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
                    memcpy_s(tmpBuf + startPos, msgSize - startPos, initValue.c_str(), initValue.size());
                    preStartPos = startPos + initValue.size();
                    outSize += initValue.size();
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

        bool frameFE::getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json)
        {
            Json::Value headJs = json["head"];
            if(headJs.isNull() || headJs["head_is_ask"].asInt() != 0x1)
                return false;

            //step1：帧头
            outValue.push_back(0xFE);

            //step2：校验码
            outValue.push_back(0x0);
            outValue.push_back(0x0);

            //step3：数据长度
            outValue.push_back(0x0);
            outValue.push_back(0x0);

            //step4：源节点编号
            if(!headJs["head_src_node"].isNull())
                outValue.push_back(headJs["head_src_node"].asInt());
            else
                outValue.push_back(0x0);

            //step5：目的节点编号
            if(!headJs["head_dst_node"].isNull())
                outValue.push_back(headJs["head_dst_node"].asInt());
            else
                outValue.push_back(0x0);

            //step6：数据种类标识吗
            if(!headJs["data_type"].isNull())
            {
                int type = headJs["data_type"].asInt();
                outValue.push_back(type >> 24);
                outValue.push_back(type >> 16);
                outValue.push_back(type >> 8);
                outValue.push_back(type & 0xFF);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            //step7：预留
            outValue.push_back(0x0);

            //step8：确认标志
            outValue.push_back(0x3);

            //step9：序号
            if(!headJs["head_cmd_cnt"].isNull())
            {
                int cnt = headJs["head_cmd_cnt"].asInt();
                outValue.push_back(cnt >> 8);
                outValue.push_back(cnt & 0xFF);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            //step10：重传次数
            if(!headJs["head_retry_cnt"].isNull())
                outValue.push_back(headJs["head_retry_cnt"].asInt());
            else
                outValue.push_back(0x0);

            //step11：更新长度及校验
            int dataLen = outValue.size() - 5;
            outValue[3] = dataLen >> 8;
            outValue[4] = dataLen & 0xFF;

            unsigned short calCrc = PfCommon::Crc::calCrc16(&outValue.at(0), outValue.size() - 3);
            outValue[1] = calCrc >> 8;
            outValue[2] = calCrc & 0xFF;

            return true;
        }

        bool frameFE::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
        {
            if(u32Size <= 0)
                return "";
            dataStorage data;
            std::ostringstream strErr;            
            Json::Value headJs;

            //step1：帧头校验
            unsigned char head = u8Msg[0];
            if(head != mHeadCode)
            {
                strErr.str("");
                strErr << "帧同步码校验错误, 接收0x" << std::hex << head << " 实际0x" << mHeadCode;
                UT_THROW_EXCEPTION(strErr.str());
            }

            //step2：计算crc校验
            unsigned short getCrc = data.getData(u8Msg, u32Size, 1, 2, 0, 0);
            unsigned short calCrc = PfCommon::Crc::calCrc16(&u8Msg[3], u32Size - 3);

            if(calCrc != getCrc)
            {
                strErr.str("");
                strErr << "帧校验和校验错误, 接收0x" << std::hex << getCrc << " 实际计算0x" << calCrc;
                UT_THROW_EXCEPTION(strErr.str());
            }

            //step3：数据长度校验
            unsigned short getLen = data.getData(u8Msg, u32Size, 1, 2, 0, 0);
            unsigned short calLen = PfCommon::Crc::calCrc16(&u8Msg[3], u32Size - 3);

            if(getLen != calLen)
            {
                strErr.str("");
                strErr << "数据长度校验错误, 接收0x" << std::hex << getLen << " 实际计算0x" << calLen;
                UT_THROW_EXCEPTION(strErr.str());
            }

            //step4：获取帧头信息
            //step4-1：获取源节点、目的节点、数据种类标识
            headJs["head_src_node"] = data.getData(u8Msg, u32Size, 5, 1, 0, 0);
            headJs["head_dst_node"] = data.getData(u8Msg, u32Size, 6, 1, 0, 0);
            headJs["data_type"] = data.getData(u8Msg, u32Size, 7, 4, 0, 0);
            headJs["head_is_ask"] = data.getData(u8Msg, u32Size, 12, 1, 0, 0);
            headJs["head_cmd_cnt"] = data.getData(u8Msg, u32Size, 13, 2, 0, 0);
            headJs["head_retry_cnt"] = data.getData(u8Msg, u32Size, 15, 1, 0, 0);

            result["head"] = headJs;

            //step3：获取表号
            int tableNum = 0;

            tableNum = data.getData(u8Msg, u32Size, 16, 2, 0, 0);

            Json::Value regionValue;
            //首地址除去表号

            _parseRegion(tableNum, &u8Msg[18], u32Size -18, regionValue);

            if(!regionValue.isNull())
                result["region"] = regionValue;

            return !result.isNull();
        }

        void frameFE::_parseRegion(const int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
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
                std::string initValue = paramValues[index][PARAM_TABLE_PARAM_BIT_SIZE].asString();

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
                return new frameFE();
            }
        }
    }
}
