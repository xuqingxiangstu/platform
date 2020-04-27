#include "framebe.h"
#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"

#include <QDateTime>

namespace Pf
{
    namespace PfIcdWorkBench
    {

        void frameBE::init(const TiXmlElement *ele)
        {

        }

        std::shared_ptr<frameObj>  frameBE::clone()
        {
            frameBE *obj = new frameBE();



            for(auto itor = mProtocolCnt.begin() ; itor != mProtocolCnt.end(); itor++)
            {
                obj->mProtocolCnt[itor->first] = (itor->second);
            }


            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void frameBE::resendMsg(byteArray &outValue)
        {
            if(outValue.size() >= 25)
                outValue.at(25) = outValue.at(25) + 1;

            if(outValue.size() > 33)
                outValue.at(33) = 0;

            unsigned short crc = PfCommon::Crc::calSum((unsigned char*)(&outValue.at(0)), outValue.size());
            dataStorage data;
            data.setData(&outValue.at(0), outValue.size(),33,1,0, 0, crc);
        }

        void frameBE::simulation(byteArray &outValue, const std::string &json)
        {
            std::ostringstream strErr;
            Json::Value headJs;
            Json::Value wordsJs;
            Json::Value regionJs;

            Json::Reader reader;
            Json::Value root;
            if(!reader.parse(json, root))
            {
                strErr.str("");
                strErr << "json格式错误(" << json << ")";

                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取帧头信息

            headJs = root["head"];
            if(headJs.isNull())
            {
                strErr.str("");
                strErr << "json 无 head 字段";

                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取信息字
            wordsJs = root["infoWord"];
            if(wordsJs.isNull())
            {
                strErr.str("");
                strErr << "json 无 infoWord 字段";

                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取域信息
            regionJs = root["region"];

            int infoType = headJs["head_info_word_type"].asInt();
            if( !( (infoType == Info_Word_0) || (infoType == Info_Word_1) || (infoType == Info_Word_2)))
            {
                strErr.str("");
                strErr << "信息字类型异常(" << std::dec << infoType << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            //填充信息字
            std::vector<byteArray> wordsMsg;
            for(int index = 0; index < wordsJs.size(); index++)
            {
                byteArray infoMsg;
                _fillInfo(infoType, infoMsg, headJs, wordsJs[index], regionJs);

                wordsMsg.push_back(infoMsg);
            }


            //填充头信息
            outValue.clear();
            fillHead(outValue, wordsMsg, headJs);
        }

        void frameBE::_fillInfo(int infoType, byteArray &outValue, Json::Value headJs, Json::Value wordsJs, Json::Value regionJs)
        {
            if(Info_Word_0 == infoType)
            {
                _fillInfoWord0(outValue, wordsJs);
            }
            else if(Info_Word_1 == infoType)
            {
                _fillInfoWord1(outValue, headJs, wordsJs, regionJs);
            }
            else if(Info_Word_2 == infoType)
            {
                _fillInfoWord2(outValue, wordsJs);
            }
        }


        void frameBE::_fillInfoWord1(byteArray &outValue, Json::Value headJs, Json::Value wordsJs, Json::Value regionJs)
        {
            dataStorage data;
            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int tmpLen = 0;
            int byteSize = 0;
            int pos = 0;

            //step1:表号
            byteSize = 2;
            if(!headJs["head_table"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, headJs["head_table"].asInt());
            }
            pos += byteSize;

            //step1：D编号
            byteSize = 2;
            if(!headJs["head_d_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, headJs["head_d_num"].asInt());
            }
            pos += byteSize;

            //step2：备用字节
            byteSize = 2;
            pos += byteSize;

            byteArray regionMsg;
            _fillRegion(regionMsg, regionJs);

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));

            std::copy(regionMsg.begin(), regionMsg.end(), std::back_inserter(outValue));
        }

        void frameBE::_fillInfoWord2(byteArray &outValue, Json::Value wordsJs)
        {
            dataStorage data;

            int dataType = 0;
            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int tmpLen = 0;
            int byteSize = 0;
            int pos = 0;

            //step1:表号
            byteSize = 2;
            if(!wordsJs["info_3_table_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_table_num"].asInt());
            }
            pos += byteSize;

            //step2：编码
            byteSize = 2;
            if(!wordsJs["info_3_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_code"].asInt());
            }
            pos += byteSize;

            //step3：设备序号
            byteSize = 1;
            if(!wordsJs["info_3_dev_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_dev_num"].asInt());
            }
            pos += byteSize;

            //step4：模块序号
            byteSize = 1;
            if(!wordsJs["info_3_module"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_module"].asInt());
            }
            pos += byteSize;

            //step5：数据类型
            byteSize = 1;
            if(!wordsJs["info_3_data_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data_type"].asInt());
                dataType = wordsJs["info_3_data_type"].asInt();
            }
            pos += byteSize;

            //step6：超差标记
            byteSize = 1;
            if(!wordsJs["info_3_over"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_over"].asInt());
            }
            pos += byteSize;

            //step7：数据值
            byteSize = 4;
            if(!wordsJs["info_3_data_value"].isNull() && !wordsJs["info_3_data_type"].isNull())
            {
                if(dataType == 1)
                    data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data_value"].asFloat());
                else if(dataType == 2)
                    data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data_value"].asInt());
            }
            pos += byteSize;

            //step8：备用
            byteSize = 2;
            pos += byteSize;

            if(3 == dataType && wordsJs["info_3_data_value"].isString())
            {
                std::string str = wordsJs["info_3_data_value"].asString();

                //step9：数据长度
                byteSize = 1;
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, str.size());
                pos += byteSize;

                //step10：数据字符串
                memcpy_s(&tmpBuf[pos], msgSize, str.c_str(), str.size());
                pos += str.size();

            }

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));
        }

        void frameBE::_fillInfoWord0(byteArray &outValue, Json::Value wordsJs)
        {
            dataStorage data;

            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int tmpLen = 0;
            int byteSize = 0;
            int pos = 0;

            //step1:表号
            byteSize = 2;
            if(!wordsJs["info_1_table_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_table_num"].asInt());
            }
            pos += byteSize;

            //step2：编码
            byteSize = 2;
            if(!wordsJs["info_1_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_code"].asInt());
            }
            pos += byteSize;

            //step3：数据
            byteSize = 4;
            if(!wordsJs["info_1_data"].isNull() && !wordsJs["info_1_data_type"].isNull())
            {
                if(wordsJs["info_1_data_type"].asInt() == 1)
                    data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data"].asFloat());
                else if(wordsJs["info_1_data_type"].asInt() == 2)
                    data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data"].asInt());
            }
            pos += byteSize;

            //step4：数据类型
            byteSize = 1;
            if(!wordsJs["info_1_data_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data_type"].asInt());
            }
            pos += byteSize;

            //step5：D编号
            byteSize = 2;
            if(!wordsJs["info_1_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_num"].asInt());
            }
            pos += byteSize;

            //step6：备用
            byteSize = 1;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, 0);
            pos += byteSize;

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));
        }

        void frameBE::fillHead(byteArray &outValue, const std::vector<byteArray> &wordMsg, Json::Value jsValue)
        {
            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int byteSize = 0;
            int index = 0;

            dataStorage data;

            //step1：帧同步
            byteSize = 1;
            tmpBuf[index] = mHeadCode;
            index += byteSize;

            //step2：帧长度
            byteSize = 2;

            index += byteSize;

            //step3：帧类型
            byteSize = 1;
            if(!jsValue["head_frame_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_frame_type"].asInt());
            }
            index += byteSize;

            //step4：信源
            byteSize = 1;
            if(!jsValue["head_src_sys_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_src_sys_type"].asInt());
            }
            index += byteSize;

            byteSize = 2;
            if(!jsValue["head_src_sys_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_src_sys_code"].asInt());
            }
            index += byteSize;

            byteSize = 1;
            if(!jsValue["head_src_node_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_src_node_code"].asInt());
            }
            index += byteSize;

            //step5：信宿
            byteSize = 1;
            if(!jsValue["head_dst_sys_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_dst_sys_type"].asInt());
            }
            index += byteSize;

            byteSize = 2;
            if(!jsValue["head_dst_sys_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_dst_sys_code"].asInt());
            }
            index += byteSize;

            byteSize = 1;
            if(!jsValue["head_dst_node_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_dst_node_code"].asInt());
            }
            index += byteSize;

            //step6：时间
            QDateTime current = QDateTime::currentDateTime();

            byteSize = 2;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, current.date().year());
            index += byteSize;

            byteSize = 1;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, current.date().month());
            index += byteSize;

            byteSize = 1;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, current.date().day());
            index += byteSize;

            unsigned int time = current.time().hour() * 3600000 + current.time().minute() * 60000 + current.time().second() * 1000 + current.time().msec();
            byteSize = 4;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, time);
            index += byteSize;

            //step7：帧序号(信源+新宿)

            unsigned int src = data.getData(tmpBuf, msgSize, 4, 4, 0, 0);
            unsigned int dst = data.getData(tmpBuf, msgSize, 8, 4, 0, 0);
            auto findItor = mProtocolCnt.find(std::make_pair(src, dst));
            if(findItor == mProtocolCnt.end())
            {
                mProtocolCnt[std::make_pair(src, dst)] = 0;
            }

            byteSize = 4;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, mProtocolCnt[std::make_pair(src, dst)]);

            mProtocolCnt[std::make_pair(src, dst)] = mProtocolCnt[std::make_pair(src, dst)] + 1;

            index += byteSize;

            //step8：：确认标志
            byteSize = 1;
            int ack = 0;
            if(!jsValue["head_table"].isNull())
            {
                Json::Value paramValues;
                paramsTable::getInstance()->getValues(jsValue["head_table"].asInt(), paramValues);

                if(paramValues.size() > 0)
                {
                    std::string isAck = paramValues[0][PARAM_TABLE_IS_ACK].asString();
                    if(IS_ACK_YES == isAck)
                        ack = 0x1;
                    else if(IS_ACK_NO == isAck)
                        ack = 0x0;
                }
            }
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, ack);
            index += byteSize;

            //step9：重传次数
            byteSize = 1;

            index += byteSize;

            //step10：信息字类型
            byteSize = 1;
            if(!jsValue["head_info_word_type"].isNull())
            {
                tmpBuf[index] = jsValue["head_info_word_type"].asInt();
            }
            index += byteSize;

            //step11：信息字个数
            byteSize = 2;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, wordMsg.size());
            index += byteSize;

            //step12：备用字节
            byteSize = 4;
            index += byteSize;

            //step13：校验和
            byteSize = 1;
            index += byteSize;

            std::copy(tmpBuf, tmpBuf + index, std::back_inserter(outValue));

            //step14：追加信息字
            for(auto msg : wordMsg)
            {
                std::copy(msg.begin(), msg.end(), std::back_inserter(outValue));
            }

            //step15：更新长度+校验
            byteSize = 2;
            data.setData(&outValue.at(0), outValue.size(), 1, byteSize, 0, 0, outValue.size());

            outValue[33] = Pf::PfCommon::Crc::calSum(&outValue.at(0), outValue.size());
        }

        void frameBE::_fillRegion(byteArray &outValue, const Json::Value &regionJs)
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

            //获取表号
            unsigned int tableNum = regionJs["table_num"].asUInt();
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
                    memcpy_s(tmpBuf + startPos, msgSize - startPos, initValue.c_str(), initValue.size());
                    preStartPos = startPos + initValue.size();
                    outSize += initValue.size();

                    //modify xqx 20200420 更新字符串长度
                    data.setData(tmpBuf, msgSize, strLenPos, strLenByteSize, 0, 0, initValue.size());
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

        bool frameBE::getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json)
        {
            Json::Value headJs = json["head"];
            if(headJs.isNull() || headJs["head_is_ask"].asInt() != 0x1)
                return false;

            if(inValue.size() < 34)
                return false;

            outValue.clear();
            std::copy(inValue.begin(), inValue.begin() + 34, std::back_inserter(outValue));

            //step1：更新确认标志
            if(outValue.size() >= 24)
                outValue.at(24) = 0x2;

            //step2：更新校验(先把校验位至0，然后再计算填充)
            if(outValue.size() >= 33)
            {
                outValue.at(33) = 0x0;

                unsigned short calCrc = PfCommon::Crc::calSum(&outValue.at(0), outValue.size());
                outValue.at(33) = calCrc & 0xFF;
            }

            return true;
        }

        void frameBE::frameCheck(unsigned char *u8Msg, const unsigned int u32Size)
        {
            std::ostringstream strErr;
            int i32Pos = 0;
            unDataConvert mData;
            mData.u64Value = 0;
            dataStorage data;

            //帧同步码
            int headSize = 1;

            if(headSize > u32Size)
            {
                strErr.str("");
                strErr << "帧同步码长度错误，headSize=" << std::dec << headSize << " framesize=" << u32Size;
                UT_THROW_EXCEPTION(strErr.str());
            }

            for (int i = headSize - 1; i >= 0; i--)
            {
                mData.i8Buf[i32Pos++] = u8Msg[i];
            }

            if (mData.u32Value != mHeadCode)
            {
                strErr.str("");
                strErr << "帧同步码校验错误, 接收0x" << std::hex << mData.u32Value << " 实际0x" << mHeadCode;
                throw std::runtime_error(strErr.str());
            }
            //帧长度校验
            unsigned int u32GetSize = data.getData(u8Msg, u32Size, 1, 2, 0, 0);

            if (u32Size != u32GetSize)
            {
                strErr.str("");
                strErr << "帧长度校验错误, 接收" << std::dec << u32Size << " 实际" << u32GetSize;
                UT_THROW_EXCEPTION(strErr.str());
            }
            int crcStartBytePos = 33;
            unsigned int calCrc = data.getData(u8Msg, u32Size, crcStartBytePos, 1, 0, 0);

            if(u32Size > crcStartBytePos)
            {
                u8Msg[crcStartBytePos] = 0;
            }

            //帧校验
            int crc = 0;

            crc = PfCommon::Crc::calSum(u8Msg, u32Size);
            //modify xqx 20200211
            crc &= 0xFF;

            if (calCrc != crc)
            {
                strErr.str("");
                strErr << "帧校验和校验错误, 接收0x" << std::hex << calCrc << " 实际计算0x" << crc;
                UT_THROW_EXCEPTION(strErr.str());
            }
        }

        int frameBE::getInfoWordSize(unsigned char *inBuf, const unsigned int inSize)
        {
            return 27;
        }

        int frameBE::getHeadSize(unsigned char *inBuf, const unsigned int inSize)
        {
            return 34;
        }

        int frameBE::getInfoWordSizeIndex(unsigned char *u8Msg, const unsigned int u32Size)
        {
            dataStorage data;

            //中间件时，信息字中间有可变参数
            int index = 38;
            int tmpLen = 0;
            //step1:消息出发点
            //step1-1：用户长度
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step1-2：软件标识
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step2:消息出发点

            index += 4;

            //step2-1：用户长度
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step2-2：软件标识
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step3:消息发送方

            //step3-1：用户长度
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step3-2：软件标识
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step4:消息接收方

            //step4-1：用户长度
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step4-2：软件标识
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;
            index += tmpLen;

            //step5：通用信息字类型
            index += 1;

            return index;
        }

        int frameBE::getInfoWordIndex(unsigned char *u8Msg, const unsigned int u32Size)
        {

            return 26;
        }

        void frameBE::_parseHead(unsigned char *inBuf, const unsigned int inSize, Json::Value &headInfo)
        {
            dataStorage data;
            int len = 0;
            int pos = 0;
            int byteSize = 0;

            pos = 3;
            //step1：帧类型
            byteSize = 1;
            headInfo["head_frame_type"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step2：信源 系统类型、系统编码、节点编码
            byteSize = 1;
            headInfo["head_src_sys_type"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 2;
            headInfo["head_src_sys_code"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_src_node_code"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step3：信宿 系统类型、系统编码、节点编码
            byteSize = 1;
            headInfo["head_dst_sys_type"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 2;
            headInfo["head_dst_sys_code"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_dst_node_code"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step3：日期+时间
            byteSize = 2;
            headInfo["head_year"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_mon"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_day"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 4;
            headInfo["head_time"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step4：命令技术+确认标志、重传次数
            byteSize = 4;
            headInfo["head_cmd_cnt"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_is_ask"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_retry_cnt"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_info_word_type"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 2;
            headInfo["head_info_word_cnt_type"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 4;
            pos += byteSize;

            byteSize = 1;
            pos += byteSize;
        }

        bool frameBE::getInfoLen(unsigned char *inBuf, const unsigned int inSize, int infoType, int &len)
        {
            bool res = false;
            len = 0;

            if(Info_Word_0 == infoType)
            {
                len = 12;
            }
            else if(Info_Word_1 == infoType)
            {
                len = inSize;
            }
            else if(Info_Word_2 == infoType)
            {
                dataStorage data;
                int typeIndex = 6;
                int lenIndex = 14;
                int lenByteSize = 1;
                int mInfoLen = 0;

                //先查看类型
                int type = data.getData(inBuf, inSize, typeIndex, 1, 0, 0);
                if(type == 3)
                {
                    //获取帧长度值
                    mInfoLen = data.getData(inBuf, inSize, lenIndex, lenByteSize, 0, 0);
                    //加上未计算长度
                    mInfoLen += lenIndex;
                    mInfoLen += lenByteSize;
                }
                else
                {
                    mInfoLen = 14;
                }

                len = mInfoLen;
            }

            if( (len != 0) && (len <= inSize))
            {
                res = true;
            }

            return res;
        }

        void frameBE::_parseInfo(int infoType, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue, Json::Value &regionJs, int tableNum)
        {
            if(Info_Word_0 == infoType)
            {
                _parseInfoWord0(u8Msg, u32Size, regionValue);
            }
            else if(Info_Word_1 == infoType)
            {
                _parseInfoWord1(u8Msg, u32Size, regionValue, regionJs, tableNum);
            }
            else if(Info_Word_2 == infoType)
            {
                _parseInfoWord2(u8Msg, u32Size, regionValue);
            }
        }

        void frameBE::_parseInfoWord0(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value)
        {
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            unDataConvert convertData;
            int dataType = 0;
            pos = 0;

            //step1：表号
            byteSize = 2;
            value["info_1_table"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step2：编码
            byteSize = 2;
            value["info_1_code"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step4：数据
            byteSize = 4;
            pos += byteSize;

            //step3：数据类型
            byteSize = 1;
            value["info_1_data_type"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            dataType = value["info_1_data_type"].asInt();
            pos += byteSize;

            //数据
            pos -= 1;
            pos -= 4;

            byteSize = 4;

            convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
            if(1 == dataType)
            {
                value["info_1_data"] = convertData.i32Value;
            }
            else if(2 == dataType)
            {
                value["info_1_data"] = convertData.f32Value;
            }
            pos += byteSize;

            pos += 1;

            //step4：D编号
            byteSize = 2;
            value["info_1_num"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;
        }

        void frameBE::_parseInfoWord1(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value, Json::Value &regionJs, int tableNum)
        {
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            unDataConvert convertData;
            int dataType = 0;
            pos = 0;

            //step1：表号
            byteSize = 2;
            tableNum = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            value["info_2_table"] = tableNum;
            pos += byteSize;

            //step1：编码
            byteSize = 2;
            value["info_2_num"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;


            //备用
            byteSize = 2;
            pos += byteSize;

            //step4：解析二进制数据
            _parseRegion(tableNum, &inBuf[pos], byteSize, regionJs);

            pos += byteSize;           
        }

        void frameBE::_parseInfoWord2(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value)
        {
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            unDataConvert convertData;
            int dataType = 0;
            pos = 0;

            //step1：表号
            byteSize = 2;
            value["info_3_table"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step2：编码
            byteSize = 2;
            value["info_3_code"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step3：设备序号
            byteSize = 1;
            value["info_3_dev_num"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step4：模块序号
            byteSize = 1;
            value["info_3_module"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step4：数据类型
            byteSize = 1;
            value["info_3_data_type"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            dataType = value["info_3_data_type"].asInt();
            pos += byteSize;

            //step5：超差标记
            byteSize = 1;
            value["info_3_over"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step6：数据
            byteSize = 4;

            if(1 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_3_data_value"] = convertData.f32Value;
            }
            else if(2 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_3_data_value"] = convertData.i32Value;
            }
            pos += byteSize;

            //step7：备用
            byteSize = 2;
            pos += byteSize;

            if(3 == dataType)
            {
                //step8：数据字符串长度
                byteSize = 1;
                value["info_3_string_len"] = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
                pos += byteSize;

                //step9：字符串
                byteSize = value["info_3_string_len"].asInt();
                if(byteSize > 0)
                {
                    value["info_3_string_value"] = std::string((const char*)&inBuf[pos], byteSize);
                }
            }
        }


        bool frameBE::getValidValue(const Json::Value &result, Json::Value &value)
        {
            bool res = false;

            //获取表号+编码
            //获取信息字类型
            if(!result["head"]["head_info_word_type"].isNull())
            {
                int type = result["head"]["head_info_word_type"].asInt();
                int tableNum = result["head"]["head_table"].asInt();
                std::string codeKey = "info_" + std::to_string(type + 1) + "_code";
                std::string tableKey = "info_" + std::to_string(type + 1) + "_table_num";

                if(!result["infoWord"].isNull())
                {
                    Json::Value infoWord = result["infoWord"];

                    for(int index = 0; index < infoWord.size(); index++)
                    {
                        Json::Value tmpJs;

                        if(!infoWord[index][codeKey].isNull())
                        {
                            tmpJs["coding"] = infoWord[index][codeKey].asInt();
                        }

                        tmpJs["table"] = tableNum;

                        value.append(tmpJs);
                    }
                }
            }

            return !value.isNull();
        }

        bool frameBE::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
        {
            dataStorage data;
            std::ostringstream strErr;

            //step1：帧校验
            frameCheck(u8Msg, u32Size);

            //step2：获取信息字类型
            unsigned short frameCode = data.getData(u8Msg, u32Size, getInfoWordIndex(u8Msg, u32Size), 1, 0, 0);

            if( !( (frameCode == Info_Word_0) || (frameCode == Info_Word_1) || (frameCode == Info_Word_2)))
            {
                strErr.str("");
                strErr << "获取信息字类型失败(" << std::dec << frameCode << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            //step3：获取信息字个数
            unsigned int infoCnt = data.getData(u8Msg, u32Size, getInfoWordSize(u8Msg, u32Size), 2, 0, 0);

            if(infoCnt <= 0)
            {
                strErr.str("");
                strErr << "获取信息字个数失败(" << std::dec << infoCnt << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            //step4：获取信息头长度
            unsigned int infoHeadSize =getHeadSize(u8Msg, u32Size);

            //step4-1：校验整帧个数是否满足条件
            /**********************************************
             * 信息头 * 信息字1 * 信息字2 * ... * 信息字n *
             **********************************************/
            if( u32Size <= infoHeadSize )
            {
                strErr.str("");
                strErr << "信息字总长度(" << std::dec << u32Size << ")" << "<=信息头长度(" << infoHeadSize << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }
            Json::Value headJson;
            Json::Value wordJsons;
            Json::Value regionJsons;

            //step5：解析信息头信息
            _parseHead(u8Msg, u32Size, headJson);

            result["head"] = headJson;

            //step6：获取各个信息字数据并解析
            int residueSize = u32Size - infoHeadSize;
            int msgPos = infoHeadSize;
            int tmpLen = 0;
            do
            {
                //step5：获取信息字数据

                //step5-1；获取信息字长度
                if(getInfoLen(&u8Msg[msgPos], residueSize, frameCode, tmpLen))
                {
                    Json::Value tmpJson;
                    //step5-2：解析信息帧
#ifndef QT_NO_DEBUG
                    std::cout << " -> ";
                    for(int index = 0; index < tmpLen; index++)
                    {
                        std::cout << std::hex << (int)u8Msg[msgPos + index] << " ";
                    }
                    std::cout << std::endl;
#endif
                    _parseInfo(frameCode, &u8Msg[msgPos], tmpLen, tmpJson, regionJsons,headJson["head_table"].asInt());

                    wordJsons.append(tmpJson);

                    //step5-3：下一帧
                    residueSize -= tmpLen;
                    msgPos += tmpLen;
                }
                else
                {
                    break;
                }
            }while(residueSize > 0);

            if(!regionJsons.isNull())
                result["region"] = regionJsons;

            if(!wordJsons.isNull())
                result["infoWord"] = wordJsons;
        }

        void frameBE::_parseRegion(const int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
        {
            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(tableNum, paramValues);

            dataStorage data;
            int preValue = 0;
            int preStartPos = 0;

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
                return new frameBE();
            }
        }
    }
}
