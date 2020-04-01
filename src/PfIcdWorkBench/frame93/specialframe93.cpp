#include "specialframe93.h"

#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"

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



            for(auto itor = mProtocolCnt.begin() ; itor != mProtocolCnt.end(); itor++)
            {
                obj->mProtocolCnt[itor->first] = (itor->second);
            }


            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
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


            //step3填充帧头
            outValue.push_back(mHeadCode);

            //step4:TODO填充命令计数
            outValue.push_back(0);

            //填充数据域
            std::copy(msg.begin(), msg.end(), std::back_inserter(outValue));

            //填充CRC
#if CHECK_CRC
            unsigned short crc = PfCommon::Crc::calCrc16(&outValue.at(1), outValue.size() - 1);
            outValue.push_back(crc >> 8);
            outValue.push_back(crc & 0xFF);
#else
            unsigned short crc = PfCommon::Crc::calSum(&outValue.at(1), outValue.size() - 1);
            outValue.push_back(crc & 0xFF);
#endif
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
            //通过表号 从数据库中获取参数信息

            //添加表号
            outValue.push_back(tableNum >> 8);
            outValue.push_back(tableNum & 0xFF);

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

        std::string specialFrame93::parse(unsigned char *u8Msg, const unsigned int u32Size)
        {
            if(u32Size <= 0)
                return "";
            dataStorage data;
            std::ostringstream strErr;

            //step1：帧头校验
            unsigned char head = u8Msg[0];
            if(head != mHeadCode)
            {
                strErr.str("");
                strErr << "帧同步码校验错误, 接收0x" << std::hex << head << " 实际0x" << mHeadCode;
                UT_THROW_EXCEPTION(strErr.str());
            }

            //step2：计算crc校验
#if CHECK_CRC
            unsigned short getCrc = data.getData(u8Msg, u32Size, u32Size - CRC_SIZE, CRC_SIZE, 0, 0);
            unsigned short calCrc = PfCommon::Crc::calCrc16(&u8Msg[1], u32Size - CRC_SIZE- 1);
#else
            unsigned short getCrc = data.getData(u8Msg, u32Size, u32Size - CRC_SIZE, CRC_SIZE, 0, 0);
            getCrc &= 0xFF;
            unsigned short calCrc = PfCommon::Crc::calSum(&u8Msg[1], u32Size - CRC_SIZE - 1);
            calCrc &= 0xFF;
#endif

            if(calCrc != getCrc)
            {
                strErr.str("");
                strErr << "帧校验和校验错误, 接收0x" << std::hex << getCrc << " 实际计算0x" << calCrc;
                UT_THROW_EXCEPTION(strErr.str());
            }

            //step3：获取表号
            int tableNum = 0;

            tableNum = data.getData(u8Msg, u32Size, 2, 2, 0, 0);

            Json::Value regionValue;
            //首地址除去表号
#if CHECK_CRC
            _parseRegion(tableNum, &u8Msg[2 + 2], u32Size - 2 - CRC_SIZE - 2, regionValue);
#else
            _parseRegion(tableNum, &u8Msg[2 + 2], u32Size - 2 - CRC_SIZE - 2, regionValue);
#endif

            return regionValue.toStyledString();
        }

        void specialFrame93::_parseRegion(const int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
        {
            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(tableNum, paramValues);

            dataStorage data;
            int preValue = 0;
            int preStartPos = 0;

            Json::Value tmpJs;

            tmpJs["table_num"] = tableNum;

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
                tmpJs["data"] = dataJs;

            regionValue["region"] = tmpJs;
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

