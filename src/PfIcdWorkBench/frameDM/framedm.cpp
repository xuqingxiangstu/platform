#include "framedm.h"

#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"

#include <QByteArray>

namespace Pf
{
    namespace PfIcdWorkBench
    {

        frameDM::frameDM()
        {
        }

        frameDM::~frameDM()
        {
        }

        void frameDM::init(const TiXmlElement *ele)
        {

        }

        std::shared_ptr<frameObj>  frameDM::clone()
        {
            frameDM *obj = new frameDM();

            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void frameDM::resendMsg(byteArray &outValue)
        {

        }

        bool frameDM::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
        {
            if(u32Size < 1)
                return false;

            dataStorage data;
            std::ostringstream strErr;
            int pos = 0;
            int byteSize = 0;

            //校验
            unsigned char crc = Pf::PfCommon::Crc::xOr((const unsigned char*)u8Msg, u32Size - 1);

            pos = u32Size - 1;
            byteSize = 1;
            unsigned char getCrc = data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);

            if(crc != getCrc)
            {
                strErr.str("");
                strErr << "帧校验错误, 接收0x" << std::hex << getCrc << " 实际 " << crc;
                UT_THROW_EXCEPTION(strErr.str());
            }

            pos = 0;
            byteSize = 1;
            result["cmd_code"] = (int)data.getData(u8Msg, u32Size, pos, byteSize, 0, 0);


            return !result.isNull();
        }

        void frameDM::simulation(byteArray &outValue, const Json::Value &root)
        {

        }

        //通用仿真
        void frameDM::simulation(byteArray &outValue, const std::string &json)
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

            byteArray tmpMsg;
            //step2-1 域仿真
            _fillRegion(tmpMsg, regionJson);

            //帧头
            outValue.push_back(0x55);
            //长度
            outValue.push_back(0x0);

            std::copy(tmpMsg.begin(), tmpMsg.end(), std::back_inserter(outValue));

            //crc
            outValue.push_back(0);

            //更新长度
            outValue[1] = outValue.size();

            //更新CRC
            int crc = 0;            
            if(outValue.size() > 0)
            {
               crc = Pf::PfCommon::Crc::calSum((const unsigned char*)&outValue.at(0), outValue.size());
            }

            outValue.push_back(crc & 0xFF);


        }

        void frameDM::_fillRegion(byteArray &outValue, const Json::Value &regionJs)
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

        extern "C"
        {
            frameObj *LoadClass()
            {
                return new frameDM();
            }
        }
    }
}
