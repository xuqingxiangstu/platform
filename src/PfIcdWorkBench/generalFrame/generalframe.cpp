#include "generalframe.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../../PfCommon/crc/crc.h"

#include "../common/subProtocol.h"
#include "../icdData/datatype.h"

#include "../icdData/datastorage.h"
#include "../icdData/dataFrame.h"
#include "../icdData/datacalc.h"
#include "../icdData/dataconvert.h"

#include "infowordconf.h"

#include "../../PfSql/paramsTable/paramstable.h"

#include <QTextCodec>
#include <QDateTime>
#include <QDebug>

namespace Pf
{
    namespace PfIcdWorkBench
    {
        generalFrame::generalFrame()
            :mFrameCfgPath(""),
             mDataRegionCfgPath(""),
             mCurFrameType(Frame_BE)
        {

        }
        generalFrame::~generalFrame()
        {

        }

        void generalFrame::setAttribute(const Json::Value &attr)
        {
            if(attr.isNull())
                return ;

            if(attr["type"].isNull())
                return ;

            std::string type = attr["type"].asString();
            if(FRAME_BE == type)
            {
                mCurFrameType = Frame_BE;
            }
            else if(FRAME_MIDDLE == type)
            {
                mCurFrameType = Frame_Middle;
            }        
        }

        int generalFrame::getMiddleInfoWordSize(unsigned char *inBuf, const unsigned int inSize)
        {
            dataStorage data;

            return data.getData(inBuf, inSize, getMiddleInfoWordIndex(inBuf, inSize) + 1, 2, 0, 0);
        }

        int generalFrame::getMiddleHeadSize(unsigned char *inBuf, const unsigned int inSize)
        {
            int index = getMiddleInfoWordIndex(inBuf, inSize);

            //信息字类型
            index += 1;

            //信息字个数
            index += 2;

            //表号
            index += 2;

            //优先级
            index += 1;

            //备用字符串长度
            dataStorage data;
            int len = data.getData(inBuf, inSize, index, 1, 0, 0);
            index += 1;

            //备用字
            index += len;

            //信息字类型
            index += 1;

            return index;
        }

        int generalFrame::getMiddleInfoWordIndex(unsigned char *u8Msg, const unsigned int u32Size)
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

            return index;
        }

        void generalFrame::getMiddleOtherHeadInfo(unsigned char *inBuf, const unsigned int inSize, Json::Value &headInfo)
        {
            dataStorage data;
            int len = 0;
            int pos = 0;
            //step1：消息出发点-用户字符串，软件标识

            pos = 34;
            headInfo["head_msg_out_sys_type"] = data.getData(inBuf, inSize, pos, 1, 0, 0);
            pos += 1;

            headInfo["head_msg_out_sys_code"] = data.getData(inBuf, inSize, pos, 1, 0, 0);
            pos += 2;

            headInfo["head_msg_out_node_code"] = data.getData(inBuf, inSize, pos, 1, 0, 0);
            pos += 1;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_out_user"] = std::string((const char*)&inBuf[pos + 1], len);
            }

            pos += 1;
            pos += len;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_out_version"] = std::string((const char*)&inBuf[pos + 1], len);
            }
            pos += 1;
            pos += len;

            //step2：消息目的地用户字符串、软件标识

            headInfo["head_msg_in_sys_type"] = data.getData(inBuf, inSize, pos, 1, 0, 0);
            pos += 1;

            headInfo["head_msg_in_sys_code"] = data.getData(inBuf, inSize, pos, 1, 0, 0);
            pos += 2;

            headInfo["head_msg_in_node_code"] = data.getData(inBuf, inSize, pos, 1, 0, 0);
            pos += 1;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_in_user"] = std::string((const char*)&inBuf[pos + 1], len);
            }

            pos += 1;
            pos += len;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_in_version"] = std::string((const char*)&inBuf[pos + 1], len);
            }

            //step3：消息发送方的地用户字符串、软件标识
            pos += 1;
            pos += len;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_send_user"] = std::string((const char*)&inBuf[pos + 1], len);
            }

            pos += 1;
            pos += len;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_send_version"] = std::string((const char*)&inBuf[pos + 1], len);
            }

            //step4：消息接收送方的地用户字符串、软件标识

            pos += 1;
            pos += len;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_rcv_user"] = std::string((const char*)&inBuf[pos + 1], len);
            }

            pos += 1;
            pos += len;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_msg_rcv_version"] = std::string((const char*)&inBuf[pos + 1], len);
            }


            //step5：消息类型（0：带表号的数据；1：自定义消息；2：日志消息）
            pos += 1;

            //step6：表号
            pos += 1;
            pos += 2;

            headInfo["head_table"] = data.getData(inBuf, inSize, pos, 2, 0, 0);

            //step7：备用字符串
            pos += 2;
            pos += 1;

            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            if(pos + 1 + len <= inSize)
            {
                headInfo["head_reserve"] = std::string((const char*)&inBuf[pos + 1], len);
            }

            //step8：更新信息字类型

            pos += 1;
            pos += len;

            headInfo["head_info_word_type"] = data.getData(inBuf, inSize, pos, 1, 0, 0);
        }

        bool generalFrame::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
        {
            std::ostringstream strErr;           
            try
            {
                //step1：帧校验
                frameCheck(u8Msg, u32Size);

                dataStorage data;
                unsigned int frameCode = 0;

                if(Frame_BE == mCurFrameType)
                {
                    // 信息字类型
                    frameCode = data.getData(u8Msg, u32Size,
                                                          mProtocolCfg->getMessage<protocolConfigure::general_info_type_start_index>(),
                                                          mProtocolCfg->getMessage<protocolConfigure::general_info_type_size_index>(),
                                                          0, 0);
                }
                else if(Frame_Middle == mCurFrameType)
                {
                    frameCode = data.getData(u8Msg, u32Size, getMiddleInfoWordIndex(u8Msg, u32Size), 1, 0, 0);
                }
                //获取信息字信息

                auto itor = mInfoWordConf.find(frameCode);

                if(itor == mInfoWordConf.end())
                {
                    strErr.str("");
                    strErr << "信息字配置不存在，帧识别码(" << std::hex << frameCode << ")";
                    UT_THROW_EXCEPTION(strErr.str());
                }
                infoConf *conf = (itor->second).get();

                //获取信息域
                auto infoRegion = mSubProtocolCfg->getRegion(frameCode);
                if(infoRegion == nullptr)
                {
                    strErr.str("");
                    strErr << "信息字域配置不存在，帧识别码(" << std::hex << frameCode << ")";
                    UT_THROW_EXCEPTION(strErr.str());
                }

                //获取信息字个数
                unsigned int infoCnt = 0;
                if(Frame_BE == mCurFrameType)
                {
                    infoCnt = data.getData(u8Msg, u32Size,
                                                          mProtocolCfg->getMessage<protocolConfigure::general_info_cnt_start_index>(),
                                                          mProtocolCfg->getMessage<protocolConfigure::general_info_cnt_size_index>(),
                                                          0, 0);
                }
                else if(Frame_Middle == mCurFrameType)
                {
                    infoCnt = getMiddleInfoWordSize(u8Msg, u32Size);
                }

                if(infoCnt <= 0)
                {
                    strErr.str("");
                    strErr << "获取信息字个数失败(" << std::dec << infoCnt << ")";
                    UT_THROW_EXCEPTION(strErr.str());
                }

                //获取信息头长度
                unsigned int infoHeadSize = 0;
                if(Frame_BE == mCurFrameType)
                {
                    infoHeadSize = mProtocolCfg->getMessage<protocolConfigure::general_infohead_size_index>();
                }
                else if(Frame_Middle == mCurFrameType)
                {
                    infoHeadSize =getMiddleHeadSize(u8Msg, u32Size);
                }

                //step2：校验整帧个数是否满足条件
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

                //step3：获取信息头信息
                //获取信息头域
                auto headRegion = mSubProtocolCfg->getRegion(headCode);
                if(headRegion != nullptr)
                {
                    _parseInfo(headRegion.get(), u8Msg, u32Size, headJson);
                }
                if(Frame_Middle == mCurFrameType)
                {
                    //获取中间件其它头信息
                    getMiddleOtherHeadInfo(u8Msg, u32Size, headJson);
                }
                result["head"] = headJson;

                //step4：获取各个信息字数据并解析
                int residueSize = u32Size - infoHeadSize;
                int msgPos = infoHeadSize;
                int tmpLen = 0;
                do
                {
                    //step5：获取信息字数据

                    //step5-1；获取信息字长度                    
                    if(conf->getInfoLen(&u8Msg[msgPos], residueSize, tmpLen))
                    {
                        Json::Value tmpJson;
                        //step5-2：解析信息帧
#ifdef DEBUG_ICD
                        std::cout << " -> ";
                        for(int index = 0; index < tmpLen; index++)
                        {
                            std::cout << std::hex << (int)u8Msg[msgPos + index] << " ";
                        }
                        std::cout << std::endl;
#endif
                        _parseInfo(infoRegion.get(), &u8Msg[msgPos], tmpLen, tmpJson);

                        wordJsons.append(tmpJson);

                        //step5-3：下一帧
                        residueSize -= tmpLen;
                        msgPos += tmpLen;

                        infoCnt--;
                    }
                    else
                    {
                        break;
                    }
                }while(infoCnt > 0);

                //step6如果为信息格式2则需解析域信息
                if(0x2 == frameCode)
                {
                    Json::Value tmpJson;
                    _parseRegion(wordJsons, &u8Msg[infoHeadSize + tmpLen], residueSize, tmpJson);

                    if(!tmpJson.isNull())
                        result["region"] = tmpJson;
                }
                if(!wordJsons.isNull())
                    result["infoWord"] = wordJsons;
            }
            catch(std::runtime_error err)
            {
                std::string errCode = err.what();
                UT_THROW_EXCEPTION(errCode);
            }

            return !result.isNull();
        }

        void generalFrame::_parseRegion(const Json::Value &wordJsons, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
        {
            //获取表号
            if(wordJsons.size() <= 0)
                return ;
            if(wordJsons[0]["info_2_table_num"].isNull())
                return ;

            int tableNum = wordJsons[0]["info_2_table_num"].asInt();
            //通过表号 从数据库中获取参数信息

            regionValue["table_num"] = tableNum;

            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(tableNum, paramValues);

            dataStorage data;
            int preValue = 0;
            int preStartPos = 0;

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

        void generalFrame::_parseInfo(const infoWordRegion *region, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
        {
            dataStorage data;
            dataCalc calc;
            __int64 pValue;
            std::ostringstream showStr;

            int preValue = 0;
            int preStartPos = 0;

            for(auto subStorage : region->mStorages)
            {
                std::string pId = subStorage->getMessage<infoWordRegion::sub_param_id_index>();
                std::string pName = subStorage->getMessage<infoWordRegion::sub_param_name_index>();
                int byte_start = subStorage->getMessage<infoWordRegion::sub_param_start_post_index>();
                int byte_size = subStorage->getMessage<infoWordRegion::sub_param_byte_size_index>();
                int bit_start = subStorage->getMessage<infoWordRegion::sub_param_bit_start_pos_index>();
                int bit_size = subStorage->getMessage<infoWordRegion::sub_param_bit_size_index>();
                std::string bigSmall = subStorage->getMessage<infoWordRegion::sub_param_small_big_index>();
                std::string strCategory = subStorage->getMessage<infoWordRegion::sub_param_category_index>();

                std::string calResult = "";

                if(byte_start == -1)
                    byte_start = preStartPos;

                if(strCategory == ncharType)
                {
                    if(byte_start >= u32Size)
                    {
                        throw std::runtime_error("起始字符异常");
                    }
                    calResult = std::string((const char*)&u8Msg[byte_start], u32Size - byte_start);

                    regionValue[pId] = calResult;

                    preStartPos = byte_start + preValue;
                }
                else
                {
                    unDataConvert pValue = data.getAutoData(u8Msg, u32Size, byte_start, byte_size, bit_start, bit_size, bigSmall);

                    preValue = pValue.i32Value;
                    preStartPos = byte_start + byte_size;

                    if( (strCategory == ieee32Type))
                    {
                        regionValue[pId] = pValue.f32Value;
                    }
                    else if( (strCategory == ieee64Type))
                    {
                        regionValue[pId] = pValue.d64Value;
                    }
                    else
                    {
                        regionValue[pId] = pValue.i32Value;
                    }
                }

            }
        }

        void generalFrame::init(const TiXmlElement *xmlEle)
        {
            const TiXmlElement *pTmpElem = NULL;

            pTmpElem = xmlEle->FirstChildElement("frameconfig");
            if (pTmpElem)
            {
                mFrameCfgPath = pTmpElem->GetText();
            }

            pTmpElem = xmlEle->FirstChildElement("dataRegionConfig");
            if (pTmpElem)
            {
                mDataRegionCfgPath = pTmpElem->GetText();
            }

            if( (mFrameCfgPath == "") || (mDataRegionCfgPath == ""))
            {
                UT_THROW_EXCEPTION( "frameconfig || dataRegionConfig == null " );
            }

            //初始化帧配置
            initFrameCfg(ROOT_PATH + mFrameCfgPath);

            //初始化子帧配置
            initSubFrameCfg(ROOT_PATH + mDataRegionCfgPath);
        }

        void generalFrame::getHeadAndWord(const std::string &json, Json::Value &head, Json::Value &words, Json::Value &regionJson, Json::Value &resendJson)
        {
            std::ostringstream strErr;

            //json反序列化
            Json::Reader reader;
            Json::Value root;
            if(!reader.parse(json, root))
            {
                strErr.str("");
                strErr << "json格式错误(" << json << ")";

                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取帧头信息

            head = root["head"];
            if(head.isNull())
            {
                strErr.str("");
                strErr << "json 无 head 字段";

                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取信息字
            words = root["infoWord"];
            if(head.isNull())
            {
                strErr.str("");
                strErr << "json 无 infoWord 字段";

                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取域信息
            regionJson = root["region"];

            //获取重传标志
            resendJson = root["resendCnt"];
        }

        bool generalFrame::beAskMsg(byteArray &outValue, const Json::Value &json)
        {
            //step1：根据输入，判断是否要应答
            if(json["head_is_ask"].isNull() || (json["head_is_ask"].asInt() != 0x1))
                return false;

            //step2：需要应答，则组帧

            outValue.clear();

            //step2-1：帧同步
            outValue.push_back(0xBE);

            //step2-2：帧长度，先占位
            outValue.push_back(0x0);
            outValue.push_back(0x0);

            //step2-3：帧类型
            if(!json["head_frame_type"].isNull())
            {
                outValue.push_back(json["head_frame_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-4：信源 系统类型，系统编码，节点编码
            if(!json["head_src_sys_type"].isNull())
            {
                outValue.push_back(json["head_src_sys_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            if(!json["head_src_sys_code"].isNull())
            {
                unsigned short tmp = json["head_src_sys_code"].asInt();                
                outValue.push_back(tmp & 0xFF);
                outValue.push_back(tmp >> 8);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            if(!json["head_src_node_code"].isNull())
            {
                outValue.push_back(json["head_src_node_code"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-5：信宿 系统类型，系统编码，节点编码
            if(!json["head_dst_sys_type"].isNull())
            {
                outValue.push_back(json["head_dst_sys_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            if(!json["head_dst_sys_code"].isNull())
            {
                unsigned short tmp = json["head_dst_sys_code"].asInt();

                outValue.push_back(tmp & 0xFF);
                outValue.push_back(tmp >> 8);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            if(!json["head_dst_node_code"].isNull())
            {
                outValue.push_back(json["head_dst_node_code"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-6：时间

            QDateTime current = QDateTime::currentDateTime();

            outValue.push_back(current.date().year() & 0xFF);
            outValue.push_back(current.date().year() >> 8);


            outValue.push_back(current.date().month());
            outValue.push_back(current.date().day());

            unsigned int time = current.time().hour() * 3600000 + current.time().minute() * 60000 + current.time().second() * 1000 + current.time().msec();
            outValue.push_back(time);

            //step2-7：确认帧序号
            if(!json["head_cmd_cnt"].isNull())
            {
                int cnt = json["head_cmd_cnt"].asInt();
                outValue.push_back(cnt & 0xFF);
                outValue.push_back(cnt >> 8);
                outValue.push_back(cnt >> 16);
                outValue.push_back(cnt >> 24);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            //step2-8：确认标志
            outValue.push_back(0x2);

            //step2-9：重传次数
            if(!json["head_retry_cnt"].isNull())
            {
                outValue.push_back(json["head_retry_cnt"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-10：信息字类型
            if(!json["head_info_word_type"].isNull())
            {
                outValue.push_back(json["head_info_word_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-11：信息字个数
            outValue.push_back(0);
            outValue.push_back(0);

            //step2-12：备用字节
            outValue.push_back(0);
            outValue.push_back(0);
            outValue.push_back(0);
            outValue.push_back(0);

            //step2-14：更新长度
            int len = outValue.size() + 1;
            outValue[2] = len & 0xFF;
            outValue[1] = len >> 8;

            //step2-13：校验和

            unsigned short crc = PfCommon::Crc::calSum((unsigned char*)(&outValue.at(0)), outValue.size());
            outValue.push_back(crc & 0xFF);

            return true;
        }

        bool generalFrame::middleAskMsg(byteArray &outValue, const Json::Value &json)
        {
            //step1：根据输入，判断是否要应答
            if(json["head_is_ask"].isNull() || (json["head_is_ask"].asInt() != 0x1))
                return false;

            //step2：需要应答，则组帧

            outValue.clear();

            //step2-1：帧同步
            outValue.push_back(0xBE);

            //step2-2：帧长度，先占位
            outValue.push_back(0x0);
            outValue.push_back(0x0);

            //step2-3：帧类型
            if(!json["head_frame_type"].isNull())
            {
                outValue.push_back(json["head_frame_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-4：信源 系统类型，系统编码，节点编码
            if(!json["head_src_sys_type"].isNull())
            {
                outValue.push_back(json["head_src_sys_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            if(!json["head_src_sys_code"].isNull())
            {
                unsigned short tmp = json["head_src_sys_code"].asInt();
                outValue.push_back(tmp & 0xFF);
                outValue.push_back(tmp >> 8);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            if(!json["head_src_node_code"].isNull())
            {
                outValue.push_back(json["head_src_node_code"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-5：信宿 系统类型，系统编码，节点编码
            if(!json["head_dst_sys_type"].isNull())
            {
                outValue.push_back(json["head_dst_sys_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            if(!json["head_dst_sys_code"].isNull())
            {
                unsigned short tmp = json["head_dst_sys_code"].asInt();
                outValue.push_back(tmp & 0xFF);
                outValue.push_back(tmp >> 8);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            if(!json["head_dst_node_code"].isNull())
            {
                outValue.push_back(json["head_dst_node_code"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-6：时间

            QDateTime current = QDateTime::currentDateTime();

            outValue.push_back(current.date().year() & 0xFF);
            outValue.push_back(current.date().year() >> 8);


            outValue.push_back(current.date().month());
            outValue.push_back(current.date().day());

            unsigned int time = current.time().hour() * 3600000 + current.time().minute() * 60000 + current.time().second() * 1000 + current.time().msec();
            outValue.push_back(time);

            //step2-7：确认帧序号
            if(!json["head_cmd_cnt"].isNull())
            {
                int cnt = json["head_cmd_cnt"].asInt();
                outValue.push_back(cnt & 0xFF);
                outValue.push_back(cnt >> 8);
                outValue.push_back(cnt >> 16);
                outValue.push_back(cnt >> 24);
            }
            else
            {
                outValue.push_back(0x0);
                outValue.push_back(0x0);
                outValue.push_back(0x0);
                outValue.push_back(0x0);
            }

            //step2-8：确认标志
            outValue.push_back(0x2);

            //step2-9：重传次数
            if(!json["head_retry_cnt"].isNull())
            {
                outValue.push_back(json["head_retry_cnt"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-10：信息字类型
            if(!json["head_info_word_type"].isNull())
            {
                outValue.push_back(json["head_info_word_type"].asInt());
            }
            else
            {
                outValue.push_back(0x0);
            }

            //step2-11：信息字个数
            outValue.push_back(0);
            outValue.push_back(0);

            //step2-12：备用字节
            outValue.push_back(0);
            outValue.push_back(0);
            outValue.push_back(0);
            outValue.push_back(0);

            //step2-14：更新长度
            int len = outValue.size() + 1;
            outValue[2] = len & 0xFF;
            outValue[1] = len >> 8;



            //step2-15：校验和

            unsigned short crc = PfCommon::Crc::calSum((unsigned char*)(&outValue.at(0)), outValue.size());
            outValue.push_back(crc & 0xFF);

            return true;
        }

        bool generalFrame::getValidValue(const Json::Value &result, Json::Value &value)
        {
            bool res = false;

            //获取表号+编码
            //获取信息字类型
            if(!result["head"]["head_info_word_type"].isNull())
            {
                int type = result["head"]["head_info_word_type"].asInt();

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

                        if(!infoWord[index][tableKey].isNull())
                        {
                            tmpJs["table"] = infoWord[index][tableKey].asInt();
                        }

                        value.append(tmpJs);
                    }
                }
            }

            return !value.isNull();
        }

        bool generalFrame::getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &resultJs)
        {
            Json::Value json = resultJs["head"];

            if(Frame_BE == mCurFrameType)
            {
                return beAskMsg(outValue, json);
            }
            else if(Frame_Middle == mCurFrameType)
            {
                return middleAskMsg(outValue, json);
            }

            return false;
        }


        void generalFrame::simulation(byteArray &outValue, const std::string &json)
        {
            std::ostringstream strErr;
            Json::Value headJs;
            Json::Value wordsJs;
            Json::Value regionJs;
            Json::Value resendJs;

            //获取帧头及信息字值
            getHeadAndWord(json, headJs, wordsJs, regionJs, resendJs);

            //获取信息字类型
            if(headJs["head_info_word_type"].isNull())
            {
                strErr.str("");
                strErr << "head 无 head_info_word_type 字段";

                UT_THROW_EXCEPTION(strErr.str());
            }

            int frameCode = headJs["head_info_word_type"].asInt();

            //获取信息字域及配置
            auto wordInfoRegion = mSubProtocolCfg->getRegion(frameCode);
            if(wordInfoRegion == nullptr)
            {
                strErr.str("");
                strErr << "信息字域配置不存在，帧识别码(" << std::hex << frameCode << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            auto wordInfoItor = mInfoWordConf.find(frameCode);

            if(wordInfoItor == mInfoWordConf.end())
            {
                strErr.str("");
                strErr << "信息字配置不存在，帧识别码(" << std::hex << frameCode << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            //获取帧头域与配置信息

            auto headInfoRegion = mSubProtocolCfg->getRegion(headCode);
            if(headInfoRegion == nullptr)
            {
                strErr.str("");
                strErr << "帧头域配置不存在，帧识别码(" << std::hex << headCode << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            auto headInfoItor = mInfoWordConf.find(headCode);

            if(headInfoItor == mInfoWordConf.end())
            {
                strErr.str("");
                strErr << "帧头字配置不存在，帧识别码(" << std::hex << headCode << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            //信息字仿真
            std::vector<byteArray> wordsMsg;

            for(int index = 0; index < wordsJs.size(); index++)
            {
                byteArray msg;
                fillData(msg, wordInfoRegion.get(), (wordInfoItor->second).get(), wordsJs[index]);
                wordsMsg.push_back(msg);
            }

            //其它域信息(帧格式为2的时候有信息域)
            byteArray regionMsg;
            if(frameCode == infoConf::InfoWord_Two)
            {
                if(!regionJs.isNull())
                {
                    fillRegion(regionMsg, regionJs);
                }
            }

            //帧头仿真
            byteArray headMsg;
            fillData(headMsg, headInfoRegion.get(), (headInfoItor->second).get(), headJs);

            //如果为中间件则进行其他填充
            Json::Value middleInfo;
            if(Frame_Middle == mCurFrameType)
            {
                fillMiddleHead(headMsg, middleInfo, headJs);
            }

            //获取确认标志
            //从数据库中获取参数信息
            Json::Value paramValues;
            int ack = 0;
            std::string tableKey = "info_" + std::to_string(frameCode + 1) + "_table_num";
            int mCurTableNum = 0;
            if(wordsJs.size() > 0)
            {
                auto tmp = wordsJs[0][tableKey].asInt();
                paramsTable::getInstance()->getValues(wordsJs[0][tableKey].asInt(), paramValues);

                if(paramValues.size() > 0)
                {
                    std::string isAck = paramValues[0][PARAM_TABLE_IS_ACK].asString();
                    if(IS_ACK_YES == isAck)
                        ack = 0x1;
                    else if(IS_ACK_NO == isAck)
                        ack = 0x0;

                    mCurTableNum = paramValues[0][PARAM_TABLE_TABLE_NUM].asInt();
                }
            }

            _simFrame(outValue, headMsg, wordsMsg, regionMsg, resendJs.asInt(), ack, mCurTableNum, middleInfo);
        }

        void generalFrame::fillRegion(byteArray &outValue, const Json::Value &regionJs)
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

        void generalFrame::fillMiddleHead(byteArray &headMsg, Json::Value &info, Json::Value jsValue)
        {
            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;

            memcpy_s(tmpBuf, msgSize, &headMsg.at(0), headMsg.size());

            dataStorage data;
            int index = 26;
            //step1：信息字类型 0x03
            tmpBuf[index] = 0x3;
            index++;

            //step2：信息字格式 0x100
            data.setData(tmpBuf, msgSize, index, 2, 0, 0, 0x100);
            index += 2;

            index += 5;

            //step3：消息出发点
            //step3-1：系统类型
            if(!jsValue["head_src_sys_type"].isNull())
            {
                tmpBuf[index] = jsValue["head_src_sys_type"].asInt();
            }
            index++;

            //step3-2：系统编码
            if(!jsValue["head_src_sys_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, 2, 0, 0, jsValue["head_src_sys_code"].asInt());
            }
            index += 2;

            //step3-3：节点编码
            if(!jsValue["head_src_node_code"].isNull())
            {
                tmpBuf[index] = jsValue["head_src_node_code"].asInt();
            }
            index++;

            //step3-4：用户长度
            std::string srcUser = "";

            if(!jsValue["head_src_user"].isNull())
            {
               srcUser = jsValue["head_src_user"].asString();
            }
            tmpBuf[index] = srcUser.size();
            index++;

            //step3-5：用户字符串
            if(srcUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcUser.c_str());
            }
            index += srcUser.size();

            //step3-6：软件标识长度
            std::string srcVersion = "";

            if(!jsValue["head_src_version"].isNull())
            {
               srcVersion = jsValue["head_src_version"].asString();
            }
            tmpBuf[index] = srcVersion.size();
            index++;

            //step3-7：软件标识
            if(srcVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcVersion.c_str());
            }
            index += srcVersion.size();

            //step4：消息目的地
            //step4-1：系统类型
            if(!jsValue["head_dst_sys_type"].isNull())
            {
                tmpBuf[index] = jsValue["head_dst_sys_type"].asInt();
            }
            index++;

            //step4-2：系统编码
            if(!jsValue["head_dst_sys_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, 2, 0, 0, jsValue["head_dst_sys_code"].asInt());
            }
            index += 2;

            //step4-3：节点编码
            if(!jsValue["head_dst_node_code"].isNull())
            {
                tmpBuf[index] = jsValue["head_dst_node_code"].asInt();
            }
            index++;

            //step4-4：用户长度
            std::string dstUser = "";

            if(!jsValue["head_dst_user"].isNull())
            {
               dstUser = jsValue["head_dst_user"].asString();
            }
            tmpBuf[index] = dstUser.size();
            index++;

            //step4-5：用户字符串
            if(dstUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstUser.c_str());
            }
            index += dstUser.size();

            //step4-6：软件标识长度
            std::string dstVersion = "";

            if(!jsValue["head_dst_version"].isNull())
            {
               dstVersion = jsValue["head_dst_version"].asString();
            }
            tmpBuf[index] = dstVersion.size();
            index++;

            //step3-7：软件标识
            if(dstVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstVersion.c_str());
            }
            index += dstVersion.size();

            //step5：消息发送方
            //step5-1：用户长度

            tmpBuf[index] = srcUser.size();
            index++;

            //step5-2：用户字符串
            if(srcUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcUser.c_str());
            }
            index += srcUser.size();

            //step5-3：软件标识长度
            tmpBuf[index] = srcVersion.size();
            index++;

            //step5-4：软件标识
            if(srcVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcVersion.c_str());
            }
            index += srcVersion.size();

            //step6：消息接收方
            //step6-1：用户长度

            tmpBuf[index] = dstUser.size();
            index++;

            //step6-2：用户字符串
            if(dstUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstUser.c_str());
            }
            index += dstUser.size();

            //step6-3：软件标识长度
            tmpBuf[index] = dstVersion.size();
            index++;

            //step6-4：软件标识
            if(dstVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstVersion.c_str());
            }
            index += dstVersion.size();

            //step7：信息字类型
            if(!jsValue["head_info_word_type"].isNull())
            {
                tmpBuf[index] = jsValue["head_info_word_type"].asInt();
            }
            index++;

            //step8：信息字个数
            info["info_cnt"] = index;
            index += 2;

            //step9：表号
            info["table"] = index;
            index += 2;

            //step10：优先级
            tmpBuf[index] = 0x2;
            index++;

            //step11：备用字符串
            std::string reserved = "reserved";
            //step11-1：长度
            tmpBuf[index] = reserved.size();
            index++;

            //step11-2：字符串
            sprintf((char*)&tmpBuf[index], "%s", reserved.c_str());
            index += reserved.size();

            info["head_size"] = index;

            headMsg.clear();
            std::copy(tmpBuf, tmpBuf + index, std::back_inserter(headMsg));
        }

        void generalFrame::fillData(byteArray &outValue, infoWordRegion *region, infoConf *conf, const Json::Value &jsValue)
        {
            if(!region || !conf)
                return ;

            dataStorage data;
            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;

            Json::Value::Members mem = jsValue.getMemberNames();
            for (auto itor = mem.begin(); itor != mem.end(); itor++)
            {
                std::string id = *itor;
                Json::Value value = jsValue[*itor];

                infoWordRegion::storageType *storage;
                if(region->getStorage(id, &storage))
                {
                    if(value.isInt())
                    {
                        data.setData(tmpBuf, msgSize,
                                     storage->getMessage<infoWordRegion::sub_param_start_post_index>(),
                                     storage->getMessage<infoWordRegion::sub_param_byte_size_index>(),
                                     storage->getMessage<infoWordRegion::sub_param_bit_start_pos_index>(),
                                     storage->getMessage<infoWordRegion::sub_param_bit_size_index>(),
                                     value.asInt());
                    }
                    else if(value.isDouble())
                    {

                        data.setData(tmpBuf, msgSize,
                                     storage->getMessage<infoWordRegion::sub_param_start_post_index>(),
                                     storage->getMessage<infoWordRegion::sub_param_byte_size_index>(),
                                     storage->getMessage<infoWordRegion::sub_param_bit_start_pos_index>(),
                                     storage->getMessage<infoWordRegion::sub_param_bit_size_index>(),
                                     value.asFloat());
                    }
                    else if(value.isString())
                    {
                        sprintf((char*)&tmpBuf[storage->getMessage<infoWordRegion::sub_param_start_post_index>()], "%s", value.asString().c_str());
                    }
                }
            }

            //获取长度
            int len = 0;
            if(conf->getInfoLen(tmpBuf, msgSize, len))
            {
                std::copy(tmpBuf, tmpBuf + len, std::back_inserter(outValue));
            }
        }

#if 0
        void generalFrame::simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode, const std::vector<icdInValueType> inValue)
        {

            std::ostringstream showStr;

            //获取信息字信息
            auto itor = mInfoWordConf.find(frameCode);

            if(itor == mInfoWordConf.end())
            {
                strErr.str("");
                strErr << "信息字配置不存在，帧识别码(" << std::hex << frameCode << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }

            infoConf *conf = (itor->second).get();

            //获取信息域
            auto infoRegion = mSubProtocolCfg->getRegion(frameCode);
            if(infoRegion == nullptr)
            {
                strErr.str("");
                strErr << "信息字域配置不存在，帧识别码(" << std::hex << frameCode << ")";
                UT_THROW_EXCEPTION(strErr.str());
            }



            std::vector<std::shared_ptr<subProtocol::subStorageType>> outStorages;
            mSubProtocolCfg->getStorages(outStorages, frameCode, insideCode);

            std::vector<icdInValueType> calInValue;
            dataConvert data;
            for(auto storage : outStorages)
            {
                std::string strCategory = "";
                double d64A = 1;
                double d64B = 0;
                double d64Lsb = 1;
                __int64 initValue = 0;
                __int64 tmpValue = 0;
                __int64 converValue = 0;

                std::string id = storage->getMessage<subProtocolConfigure::sub_param_id_index>();
                strCategory = storage->getMessage<subProtocolConfigure::sub_param_category_index>();
                d64A = storage->getMessage<subProtocolConfigure::sub_param_a_index>();
                d64B = storage->getMessage<subProtocolConfigure::sub_param_b_index>();
                d64Lsb = storage->getMessage<subProtocolConfigure::sub_param_lsb_index>();

                initValue = storage->getMessage<subProtocolConfigure::sub_init_value_index>();

                /// 如果有输入数据则按照输入数据计算，否则按照总参数表默认数据
                auto itor = std::find_if(inValue.begin(), inValue.end(),
                                         [=](const icdInValueType &inV)
                {
                    return std::get<0>(inV) == id;
                }

                                         );

                if(itor != inValue.end())
                {
                    tmpValue = std::get<1>((*itor));
                }
                else
                {
                    tmpValue = initValue;
                }

                /// 按照公式进行计算
                converValue = data.getData(strCategory, tmpValue, d64A, d64B, d64Lsb);

                calInValue.push_back(std::make_pair(id, converValue));
            }

            /// step2 数据仿真
            _simulation(outValue, frameCode, insideCode, calInValue);

        }
#endif
        /*void generalFrame::_simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode, const std::vector<icdInValueType> inValue)
        {
#if 0
            std::ostringstream strErr;
            int frameLen = 0;
            int maxByteSize = 0;
            int beyond = 0;
            unsigned char tmpBuf[2048] = {0};
            dataFrame fParse;
            dataStorage data;

            /// step1 填充子帧信息
            if(!mSubProtocolCfg->isExist(frameCode, insideCode))
            {
                strErr << "帧识别码(0x" << std::hex << frameCode << "，" << insideCode << ")不存在";
                UT_THROW_EXCEPTION(strErr.str());
            }

            frameLen = mSubProtocolCfg->getMsgLen(frameCode, insideCode);
            maxByteSize = mSubProtocolCfg->getMaxByteSize(frameCode, insideCode);
            beyond = mSubProtocolCfg->getMsgBeyond(frameCode, insideCode);

            if(frameLen < maxByteSize)
            {
                strErr << "帧识别码(0x" << std::hex << frameCode << "，" << insideCode << ")，帧长度(" << std::dec << frameLen << ")与参数位(" << maxByteSize << ")索引异常";
                UT_THROW_EXCEPTION(strErr.str());
            }
            std::vector<std::shared_ptr<subProtocolConfigure::subStorageType>> outStorages;
            if(!mSubProtocolCfg->getStorages(outStorages, frameCode, insideCode))
            {
                strErr << "帧识别码(0x" << std::hex << frameCode << "，" << insideCode << ")，获取子帧信息失败";
                UT_THROW_EXCEPTION(strErr.str());
            }

            fParse.simulation(tmpBuf, frameLen, outStorages, beyond, inValue);

            try
            {
                /// step2 添加协议

                /// 帧头
                data.setData(tmpBuf, frameLen, 0, mProtocolCfg->getMessage<protocolConfigure::variable_head_size_index>(), 0, 0,
                             mProtocolCfg->getMessage<protocolConfigure::variable_head_init_index>());

                /// 帧长度
                data.setData(tmpBuf, frameLen, mProtocolCfg->getMessage<protocolConfigure::variable_frame_len_start_index>(),
                             mProtocolCfg->getMessage<protocolConfigure::variable_frame_len_byte_size_index>(), 0, 0,
                             frameLen - mProtocolCfg->getMessage<protocolConfigure::variable_cal_len_start_index>() - mProtocolCfg->getMessage<protocolConfigure::variable_cal_len_to_end_index>());

                /// 帧标识
                data.setData(tmpBuf, frameLen, mProtocolCfg->getMessage<protocolConfigure::variable_code_start_index>(),
                             mProtocolCfg->getMessage<protocolConfigure::variable_code_size_index>(),
                             mProtocolCfg->getMessage<protocolConfigure::variable_code_bit_start_index>(),
                             mProtocolCfg->getMessage<protocolConfigure::variable_code_bit_size_index>(),
                             frameCode);

                /// 命令计数
                data.setData(tmpBuf, frameLen, frameLen - mProtocolCfg->getMessage<protocolConfigure::variable_cnt_to_end_index>(),
                             mProtocolCfg->getMessage<protocolConfigure::variable_cnt_size_index>(), 0, 0,
                             mProtocolCnt[std::make_pair(frameCode, insideCode)]);

                mProtocolCnt[std::make_pair(frameCode, insideCode)] = mProtocolCnt[std::make_pair(frameCode, insideCode)] + 1;
             }
            catch(std::runtime_error err)
            {
                strErr << err.what();
                UT_THROW_EXCEPTION(strErr.str());
            }

            /// crc
            upDataCrc(tmpBuf, frameLen);

            /// 消息传递给上层
            outValue.clear();
            std::copy(tmpBuf, tmpBuf + frameLen, std::back_inserter(outValue));
#endif
        }
*/
        void generalFrame::upDataCrc(unsigned char *u8Msg, const unsigned int u32Size)
        {
#if 0
            std::ostringstream strErr;
            //帧校验
            int crc = 0;
            int checkStartPos = mProtocolCfg->getMessage<protocolConfigure::variable_cal_check_start_index>();
            int checkSize = u32Size - checkStartPos - mProtocolCfg->getMessage<protocolConfigure::variable_cal_check_to_end_index>();

            if( (checkStartPos + checkSize) > u32Size)
            {
                strErr.str("");
                strErr << "帧校验索引异常, checkStart=" << std::dec << checkStartPos << " checkSize=" << checkSize << " > frameSize=" << u32Size;
                UT_THROW_EXCEPTION(strErr.str());
            }

            std::string cType = mProtocolCfg->getMessage<protocolConfigure::variable_check_type_index>();
            if (cType == CRC_CHECK)
            {
                crc = PfCommon::Crc::calCrc16((unsigned char*)(&u8Msg[checkStartPos]), checkSize);
            }
            else if (cType == SUM_CHECK)
            {
                crc = PfCommon::Crc::calSum((unsigned char*)(&u8Msg[checkStartPos]), checkSize);
                crc &= 0xF;
            }

            dataStorage data;
            data.setData(u8Msg, u32Size, u32Size - mProtocolCfg->getMessage<protocolConfigure::variable_frame_check_to_end_index>(),
                         mProtocolCfg->getMessage<protocolConfigure::variable_frame_check_size_index>(), 0, 0, crc);
#endif
        }

        std::shared_ptr<frameObj> generalFrame::clone()
        {
            generalFrame *obj = new generalFrame();

            obj->mProtocolCfg = this->mProtocolCfg->clone();
            obj->mSubProtocolCfg = this->mSubProtocolCfg->clone();

            for(auto itor = mInfoWordConf.begin() ; itor != mInfoWordConf.end(); itor++)
            {
                obj->mInfoWordConf[itor->first] = (itor->second)->clone();
            }

            for(auto itor = mProtocolCnt.begin() ; itor != mProtocolCnt.end(); itor++)
            {
                obj->mProtocolCnt[itor->first] = (itor->second);
            }

            obj->mFrameCfgPath = this->mFrameCfgPath;
            obj->mDataRegionCfgPath = this->mDataRegionCfgPath;

            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void generalFrame::frameCheck(unsigned char *u8Msg, const unsigned int u32Size)
        {
            std::ostringstream strErr;
            int i32Pos = 0;
            unDataConvert mData;
            mData.u64Value = 0;
            dataStorage data;

            //帧同步码
            int headSize = mProtocolCfg->getMessage<protocolConfigure::general_head_size_index>();

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

            if (mData.u32Value != mProtocolCfg->getMessage<protocolConfigure::general_head_init_index>())
            {
                strErr.str("");
                strErr << "帧同步码校验错误, 接收0x" << std::hex << mData.u32Value << " 实际0x" << mProtocolCfg->getMessage<protocolConfigure::general_head_init_index>();
                throw std::runtime_error(strErr.str());
            }
            //帧长度校验
            unsigned int u32GetSize = data.getData(u8Msg, u32Size, mProtocolCfg->getMessage<protocolConfigure::general_frame_len_start_index>(),
                                                   mProtocolCfg->getMessage<protocolConfigure::general_frame_len_byte_size_index>(), 0, 0);

            u32GetSize = u32GetSize + mProtocolCfg->getMessage<protocolConfigure::general_cal_len_start_index>() + mProtocolCfg->getMessage<protocolConfigure::general_cal_len_to_end_index>();

            if (u32Size != u32GetSize)
            {
                strErr.str("");
                strErr << "帧长度校验错误, 接收" << std::dec << u32Size << " 实际" << u32GetSize;
                UT_THROW_EXCEPTION(strErr.str());
            }

            unsigned int calCrc = data.getData(u8Msg, u32Size,
                                               mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>(),
                                                  mProtocolCfg->getMessage<protocolConfigure::general_frame_check_size_index>(), 0, 0);

            if(u32Size > mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>())
            {
                u8Msg[mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>()] = 0;
            }

            //帧校验
            int crc = 0;
            int checkStartPos = mProtocolCfg->getMessage<protocolConfigure::general_cal_check_start_index>();
            int checkSize = u32Size - checkStartPos - mProtocolCfg->getMessage<protocolConfigure::general_cal_check_to_end_index>();

            if( (checkStartPos + checkSize) > u32Size)
            {
                strErr.str("");
                strErr << "帧校验索引异常, checkStart=" << std::dec << checkStartPos << " checkSize=" << checkSize << " > frameSize=" << u32Size;
                UT_THROW_EXCEPTION(strErr.str());
            }

            std::string cType = mProtocolCfg->getMessage<protocolConfigure::general_check_type_index>();
            if (cType == CRC_CHECK)
            {
                crc = PfCommon::Crc::calCrc16((unsigned char*)(&u8Msg[checkStartPos]), checkSize);
            }
            else if (cType == SUM_CHECK)
            {
                u8Msg[mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>()] = 0;
                crc = PfCommon::Crc::calSum(u8Msg, u32Size);
                //modify xqx 20200211
                crc &= 0xFF;
            }

            if (calCrc != crc)
            {
                strErr.str("");
                strErr << "帧校验和校验错误, 接收0x" << std::hex << calCrc << " 实际计算0x" << crc;
                UT_THROW_EXCEPTION(strErr.str());
            }
        }

        void generalFrame::initFrameCfg(const std::string &path)
        {
            std::ostringstream strErr;

    #ifdef Utf8_Coding
            QXlsx::Document xlsx(QString::fromUtf8(path.c_str()));
    #else
            QXlsx::Document xlsx(QString::fromLocal8Bit(path.c_str()));
    #endif
            QXlsx::Workbook *workBook = xlsx.workbook();

            QStringList sheetNames = xlsx.sheetNames();

            if(sheetNames.size() == 0)
            {
                strErr.str("");
                strErr << "file path = (" << path << " ) sheet == 0";
                UT_THROW_EXCEPTION(strErr.str());
            }

            //帧格式配置信息
            mProtocolCfg = std::make_shared<protocolConfigure>();
            mProtocolCfg->init(static_cast<QXlsx::Worksheet*>(workBook->sheet(0)));

            //信息字帧格式配置
            //TODO 改成加载配置文件
            infoConf *cfg = nullptr;

            cfg = new infoWord1Conf();
            mInfoWordConf[cfg->getInfoType()] = std::shared_ptr<infoConf>(cfg);

            cfg = new infoWord2Conf();
            mInfoWordConf[cfg->getInfoType()] = std::shared_ptr<infoConf>(cfg);

            cfg = new infoWord3Conf();
            mInfoWordConf[cfg->getInfoType()] = std::shared_ptr<infoConf>(cfg);

            cfg = new frameHeadConf();
            mInfoWordConf[cfg->getInfoType()] = std::shared_ptr<infoConf>(cfg);
        }

        void generalFrame::initSubFrameCfg(const std::string &path)
        {
            std::ostringstream strErr;

    #ifdef Utf8_Coding
            QXlsx::Document xlsx(QString::fromUtf8(path.c_str()));
    #else
            QXlsx::Document xlsx(QString::fromLocal8Bit(path.c_str()));
    #endif
            QXlsx::Workbook *workBook = xlsx.workbook();

            QStringList sheetNames = xlsx.sheetNames();

            if(sheetNames.size() == 0)
            {
                strErr.str("");
                strErr << "file path = (" << path << " ) sheet == 0";
                UT_THROW_EXCEPTION(strErr.str());
            }

            if(sheetNames.size() == 1)
            {
                strErr.str("");
                strErr << "file path = (" << path << " ) sheet == 1 无子帧配置";
                UT_THROW_EXCEPTION(strErr.str());
            }

            mSubProtocolCfg = std::make_shared<infoWordRegionManager>();
            mSubProtocolCfg->init(workBook);
        }

        void generalFrame::resendMsg(byteArray &outValue)
        {
            if(outValue.size() >= 25)
                outValue.at(25) = outValue.at(25) + 1;

            if(outValue.size() > 33)
                outValue.at(mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>()) = 0;

            unsigned short crc = PfCommon::Crc::calSum((unsigned char*)(&outValue.at(0)), outValue.size());
            dataStorage data;
            data.setData(&outValue.at(0), outValue.size(),
                         mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>(),
                         mProtocolCfg->getMessage<protocolConfigure::general_frame_check_size_index>(),
                         0, 0, crc);
        }

        void generalFrame::_simFrame(byteArray &outValue, const byteArray &headValue, const std::vector<byteArray> &wordsValue, const byteArray &regionValue, const int &resendCnt, const int &ack, const int &tableNum, const Json::Value &middleJson)
        {
            unsigned char tmpBuf[1024] = {0};
            dataStorage data;

            outValue.clear();

            //step1 填充信息头
            std::copy(headValue.begin(), headValue.end(), std::back_inserter(outValue));

            //step1-1：获取信息头长度
            int headLen = 0;
            if(Frame_BE == mCurFrameType)
            {
                headLen = mProtocolCfg->getMessage<protocolConfigure::general_infohead_size_index>();
            }
            else if(Frame_Middle == mCurFrameType)
            {
                if( !middleJson.isNull() && !middleJson["head_size"].isNull())
                {
                    headLen = middleJson["head_size"].asInt();
                }
            }

            //step1-2：帧同步
            data.setData(&outValue.at(0), headLen, 0, 1, 0, 0, mProtocolCfg->getMessage<protocolConfigure::general_head_init_index>());
/*
            //step1-3；帧类型
            data.setData(tmpBuf, headLen, mProtocolCfg->getMessage<protocolConfigure::general_code_start_index>(),
                         mProtocolCfg->getMessage<protocolConfigure::general_code_size_index>(),
                         0, 0, frameType);
*/
            //step1-4：填充时间
            QDateTime current = QDateTime::currentDateTime();

            data.setData(&outValue.at(0), headLen, 12, 2, 0, 0, current.date().year());
            data.setData(&outValue.at(0), headLen, 14, 1, 0, 0, current.date().month());
            data.setData(&outValue.at(0), headLen, 15, 1, 0, 0, current.date().day());
            unsigned int time = current.time().hour() * 3600000 + current.time().minute() * 60000 + current.time().second() * 1000 + current.time().msec();
            data.setData(&outValue.at(0), headLen, 16, 4, 0, 0, time);

            /*//step1-5：信息字类型
            data.setData(tmpBuf, headLen, mProtocolCfg->getMessage<protocolConfigure::general_info_type_start_index>(),
                         mProtocolCfg->getMessage<protocolConfigure::general_info_type_size_index>(),
                         0, 0, infoWordType);
*/
            //step1-6：信息字个数
            if(Frame_BE == mCurFrameType)
            {
                data.setData(&outValue.at(0), headLen, mProtocolCfg->getMessage<protocolConfigure::general_info_cnt_start_index>(),
                             mProtocolCfg->getMessage<protocolConfigure::general_info_cnt_size_index>(),
                             0, 0, wordsValue.size());
            }
            else if(Frame_Middle == mCurFrameType)
            {
                if( !middleJson.isNull() && !middleJson["info_cnt"].isNull())
                {
                    data.setData(&outValue.at(0), headLen, middleJson["info_cnt"].asInt(),
                                 mProtocolCfg->getMessage<protocolConfigure::general_info_cnt_size_index>(),
                                 0, 0, wordsValue.size());
                }
            }

            //step1-7:填充表号
            if( (Frame_Middle == mCurFrameType) && !middleJson.isNull() && !middleJson["table"].isNull())
            {
                data.setData(&outValue.at(0), headLen, middleJson["table"].asInt(), 2, 0, 0, tableNum);
            }

            //step1-7：帧序号(信源+新宿)

            unsigned int src = data.getData(&outValue.at(0), headLen, 4, 4, 0, 0);
            unsigned int dst = data.getData(&outValue.at(0), headLen, 8, 4, 0, 0);

            auto findItor = mProtocolCnt.find(std::make_pair(src, dst));
            if(findItor == mProtocolCnt.end())
            {
                mProtocolCnt[std::make_pair(src, dst)] = 0;
            }

            data.setData(&outValue.at(0), headLen, mProtocolCfg->getMessage<protocolConfigure::general_cnt_start_index>(),
                         mProtocolCfg->getMessage<protocolConfigure::general_cnt_size_index>(), 0, 0,
                         mProtocolCnt[std::make_pair(src, dst)]);

            if(resendCnt == 0)//重传时不需要更新命令计数
                mProtocolCnt[std::make_pair(src, dst)] = mProtocolCnt[std::make_pair(src, dst)] + 1;

            //step1-8：确认标志
            data.setData(&outValue.at(0), headLen, 24, 1, 0, 0, ack);

            //step1-9：重传次数
            data.setData(&outValue.at(0), headLen, 25, 1, 0, 0, resendCnt);

            //std::copy(tmpBuf, tmpBuf + headLen, std::back_inserter(outValue));

            //step2：信息字
            for(auto v : wordsValue)
            {
                std::copy(v.begin(), v.end(), std::back_inserter(outValue));
            }

            //step3：域信息（信息格式为2时）
            if(regionValue.size() > 0)
                std::copy(regionValue.begin(), regionValue.end(), std::back_inserter(outValue));

            //step3：更新长度
            data.setData(&outValue.at(0), outValue.size(), mProtocolCfg->getMessage<protocolConfigure::general_frame_len_start_index>(),
                                     mProtocolCfg->getMessage<protocolConfigure::general_frame_len_byte_size_index>(),
                                     0, 0, int(outValue.size() - mProtocolCfg->getMessage<protocolConfigure::general_cal_len_start_index>() - mProtocolCfg->getMessage<protocolConfigure::general_cal_len_to_end_index>()));

            //step4：更新校验和(除自身外其余累加和)
            outValue.at(mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>()) = 0;

            unsigned short crc = PfCommon::Crc::calSum((unsigned char*)(&outValue.at(0)), outValue.size());

            data.setData(&outValue.at(0), outValue.size(),
                         mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>(),
                         mProtocolCfg->getMessage<protocolConfigure::general_frame_check_size_index>(),
                         0, 0, crc);
        }

        extern "C"
        {
            frameObj *LoadClass()
            {
                return new generalFrame();
            }
        }
    }
}
