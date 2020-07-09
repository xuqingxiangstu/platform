#include "framemiddle.h"


#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../../PfCommon/tools/ut_error.h"
#include "../icdData/datatype.h"
#include "../../PfSql/paramsTable/paramstable.h"
#include "../frameNumber/framenumber.h"

#include <QByteArray>
#include <QDebug>
#include <QDateTime>

namespace Pf
{
    namespace PfIcdWorkBench
    {

        frameMiddle::frameMiddle():
            mAskFlag(0x1)
        {
        }

        frameMiddle::~frameMiddle()
        {
        }

        void frameMiddle::init(const TiXmlElement *ele)
        {

        }

        std::shared_ptr<frameObj>  frameMiddle::clone()
        {
            frameMiddle *obj = new frameMiddle();

            obj->mAskFlag = this->mAskFlag;

            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void frameMiddle::resendMsg(byteArray &outValue)
        {
            if(outValue.size() >= 25)
                outValue.at(25) = outValue.at(25) + 1;

            if(outValue.size() > 33)
                outValue.at(33) = 0;

            unsigned short crc = PfCommon::Crc::calSum((unsigned char*)(&outValue.at(0)), outValue.size());
            dataStorage data;
            data.setData(&outValue.at(0), outValue.size(),33,1,0, 0, crc);
        }

        void frameMiddle::simulation(byteArray &outValue, const std::string &json)
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

        void frameMiddle::_fillInfo(int infoType, byteArray &outValue, Json::Value headJs, Json::Value wordsJs, Json::Value regionJs)
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


        void frameMiddle::_fillInfoWord1(byteArray &outValue, Json::Value headJs, Json::Value wordsJs, Json::Value regionJs)
        {
            dataStorage data;
            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int tmpLen = 0;
            int byteSize = 0;
            int pos = 0;

            byteArray regionMsg;

            //note:帧格式类型在帧头里进行填充，此处不进行填充 2020-4-24 15:35:36
           // byteSize = 1;
            //data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, 1);
            //pos += byteSize;

            //step1：D编号
            byteSize = 2;
            if(!headJs["head_d_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, headJs["head_d_num"].asInt());
            }
            pos += byteSize;

            _fillRegion(regionMsg, regionJs);

            //step2：数据长度
            byteSize = 2;
            data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, (int)regionMsg.size());
            pos += byteSize;

            //step3：二进制数据
            byteSize = regionMsg.size();
            if(byteSize > 0)
                memcpy(&tmpBuf[pos], &regionMsg.at(0), regionMsg.size());
            pos += byteSize;

            //step4：备用长度
            byteSize = 1;
            if(!wordsJs["info_2_reserve_len"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_2_reserve_len"].asInt());
            }
            pos += byteSize;

            //step5：备用字符串
            if(!wordsJs["info_2_reserve"].isNull())
            {
                std::string str = wordsJs["info_2_reserve"].asString();
                memcpy(&tmpBuf[pos],  str.c_str(), str.size());
                pos += str.size();
            }

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));
        }

        void frameMiddle::_fillInfoWord2(byteArray &outValue, Json::Value wordsJs)
        {
            dataStorage data;

            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int tmpLen = 0;
            int byteSize = 0;
            int pos = 0;

            //step1：编码
            byteSize = 2;
            if(!wordsJs["info_3_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_code"].asInt());
            }
            pos += byteSize;

            //step2：设备序号
            byteSize = 1;
            if(!wordsJs["info_3_dev_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_dev_num"].asInt());
            }
            pos += byteSize;

            //step3：模块序号
            byteSize = 1;
            if(!wordsJs["info_3_module"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_module"].asInt());
            }
            pos += byteSize;

            //step4：数据类型
            byteSize = 1;
            if(!wordsJs["info_3_data_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data_type"].asInt());
            }
            pos += byteSize;

            //step5：超差标记
            byteSize = 1;
            if(!wordsJs["info_3_over"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_over"].asInt());
            }
            pos += byteSize;

            //step6：D编号
            byteSize = 2;
            if(!wordsJs["info_3_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_num"].asInt());
            }
            pos += byteSize;

            //step7：数据长度
            byteSize = 1;
            if(!wordsJs["info_3_data_len"].isNull())
            {
                tmpLen = wordsJs["info_3_data_len"].asInt();
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data_len"].asInt());
            }
            else
            {
                tmpLen = 0;
            }
            pos += byteSize;

            //step8：数据
            byteSize = tmpLen;
            if(!wordsJs["info_3_data"].isNull() && byteSize > 0)
            {
                Json::Value tmpJs = wordsJs["info_3_data"];
                if(tmpJs.isInt())
                {
                    data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data"].asInt());
                }
                else if(tmpJs.isDouble())
                {
                    if(4 == byteSize)
                    {
                        data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data"].asFloat());
                    }
                    else if(8 == byteSize)
                    {
                        data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_data"].asDouble());
                    }
                }
                else if(tmpJs.isString())
                {
                    memcpy(&tmpBuf[pos], tmpJs.asString().c_str(), tmpJs.asString().size());
                }
            }
            pos += byteSize;

            //step9：备用长度
            byteSize = 1;
            if(!wordsJs["info_3_reserve_len"].isNull())
            {
                tmpLen = wordsJs["info_3_reserve_len"].asInt();
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_3_reserve_len"].asInt());
            }
            else
            {
                tmpLen = 0;
            }
            pos += byteSize;

            //step10：备用字符串
            byteSize = tmpLen;

            if(tmpLen > 0 && wordsJs["info_3_reserve"].isString())
            {
                memcpy(&tmpBuf[pos],  wordsJs["info_3_reserve"].asString().c_str(), wordsJs["info_3_reserve"].asString().size());
            }
            pos += byteSize;

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));
        }

        void frameMiddle::_fillInfoWord0(byteArray &outValue, Json::Value wordsJs)
        {
            dataStorage data;

            unsigned char tmpBuf[1024] = {0};
            unsigned int msgSize = 512;
            int tmpLen = 0;
            int byteSize = 0;
            int pos = 0;

            //step1：编码
            byteSize = 2;
            if(!wordsJs["info_1_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_code"].asInt());
            }
            pos += byteSize;

            //step2：数据类型
            byteSize = 1;
            if(!wordsJs["info_1_data_type"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data_type"].asInt());
            }
            pos += byteSize;

            //step3：超差标记
            byteSize = 1;
            if(!wordsJs["info_1_over"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_over"].asInt());
            }
            pos += byteSize;

            //step4：D编号
            byteSize = 2;
            if(!wordsJs["info_1_num"].isNull())
            {
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_num"].asInt());
            }
            pos += byteSize;

            //step5：数据长度
            byteSize = 1;
            if(!wordsJs["info_1_data_len"].isNull())
            {
                tmpLen = wordsJs["info_1_data_len"].asInt();
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data_len"].asInt());
            }
            else
            {
                tmpLen = 0;
            }
            pos += byteSize;

            //step6：数据
            byteSize = tmpLen;
            if(!wordsJs["info_1_data"].isNull() && byteSize > 0)
            {
                Json::Value tmpJs = wordsJs["info_1_data"];
                if(tmpJs.isInt())
                {
                    data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data"].asInt());
                }
                else if(tmpJs.isDouble())
                {
                    if(4 == byteSize)
                    {
                        data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data"].asFloat());
                    }
                    else if(8 == byteSize)
                    {
                        data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_data"].asDouble());
                    }
                }
                else if(tmpJs.isString())
                {
                    memcpy(&tmpBuf[pos], tmpJs.asString().c_str(), tmpJs.asString().size());
                }
            }
            pos += byteSize;

            //step7：备用长度
            byteSize = 1;
            if(!wordsJs["info_1_reserve_len"].isNull())
            {
                tmpLen = wordsJs["info_1_reserve_len"].asInt();
                data.setData(tmpBuf, msgSize, pos, byteSize, 0, 0, wordsJs["info_1_reserve_len"].asInt());
            }
            else
            {
                tmpLen = 0;
            }
            pos += byteSize;

            //step8：备用字符串
            byteSize = tmpLen;

            if(tmpLen > 0 && wordsJs["info_1_reserve"].isString())
            {
                memcpy(&tmpBuf[pos],  wordsJs["info_1_reserve"].asString().c_str(), wordsJs["info_1_reserve"].asString().size());
            }
            pos += byteSize;

            std::copy(tmpBuf, tmpBuf + pos, std::back_inserter(outValue));
        }

        void frameMiddle::setAttribute(const Json::Value &attr)
        {
            if(attr.isNull())
                return ;

            if(attr["head_ask_flag"].isNull())
                return;

            QString tmpValue = QString::fromStdString(attr["head_ask_flag"].asString());

            bool isOk = false;

            unsigned int tmpV = tmpValue.toInt(&isOk, 10);

            if(!isOk)
                tmpV = tmpValue.toUInt(&isOk, 16);

            if(isOk)
                mAskFlag = tmpV;
        }

        void frameMiddle::fillHead(byteArray &outValue, const std::vector<byteArray> &wordMsg, Json::Value jsValue)
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

            int cnt = frameNumberManager::getInstance()->getFrameNumber(mCurUuid, getFrameName(), src, dst);

            byteSize = 4;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, cnt);

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
                        ack = mAskFlag;
                    else if(IS_ACK_NO == isAck)
                        ack = 0x0;
                }
            }
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, ack);
            index += byteSize;

            //step9：重传次数
            byteSize = 1;

            index += byteSize;

            //step10：信息字类型 0x03
            tmpBuf[index] = 0x3;
            index++;

            //step11：信息字格式 0x100
            data.setData(tmpBuf, msgSize, index, 2, 0, 0, 0x100);
            index += 2;

            index += 5;

            //step12：消息出发点
            //step12-1：系统类型
            if(!jsValue["head_src_sys_type"].isNull())
            {
                tmpBuf[index] = jsValue["head_src_sys_type"].asInt();
            }
            index++;

            //step12-2：系统编码
            if(!jsValue["head_src_sys_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, 2, 0, 0, jsValue["head_src_sys_code"].asInt());
            }
            index += 2;

            //step12-3：节点编码
            if(!jsValue["head_src_node_code"].isNull())
            {
                tmpBuf[index] = jsValue["head_src_node_code"].asInt();
            }
            index++;

            //step12-4：用户长度
            std::string srcUser = "";

            if(!jsValue["head_src_user"].isNull())
            {
               srcUser = jsValue["head_src_user"].asString();
            }
            tmpBuf[index] = srcUser.size();
            index++;

            //step12-5：用户字符串
            if(srcUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcUser.c_str());
            }
            index += srcUser.size();

            //step12-6：软件标识长度
            std::string srcVersion = "";

            if(!jsValue["head_src_version"].isNull())
            {
               srcVersion = jsValue["head_src_version"].asString();
            }
            tmpBuf[index] = srcVersion.size();
            index++;

            //step12-7：软件标识
            if(srcVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcVersion.c_str());
            }
            index += srcVersion.size();

            //step13：消息目的地
            //step13-1：系统类型
            if(!jsValue["head_dst_sys_type"].isNull())
            {
                tmpBuf[index] = jsValue["head_dst_sys_type"].asInt();
            }
            index++;

            //step13-2：系统编码
            if(!jsValue["head_dst_sys_code"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, 2, 0, 0, jsValue["head_dst_sys_code"].asInt());
            }
            index += 2;

            //step13-3：节点编码
            if(!jsValue["head_dst_node_code"].isNull())
            {
                tmpBuf[index] = jsValue["head_dst_node_code"].asInt();
            }
            index++;

            //step13-4：用户长度
            std::string dstUser = "";

            if(!jsValue["head_dst_user"].isNull())
            {
               dstUser = jsValue["head_dst_user"].asString();
            }
            tmpBuf[index] = dstUser.size();
            index++;

            //step13-5：用户字符串
            if(dstUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstUser.c_str());
            }
            index += dstUser.size();

            //step13-6：软件标识长度
            std::string dstVersion = "";

            if(!jsValue["head_dst_version"].isNull())
            {
               dstVersion = jsValue["head_dst_version"].asString();
            }
            tmpBuf[index] = dstVersion.size();
            index++;

            //step13-7：软件标识
            if(dstVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstVersion.c_str());
            }
            index += dstVersion.size();

            //step14：消息发送方
            //step14-1：用户长度

            tmpBuf[index] = srcUser.size();
            index++;

            //step14-2：用户字符串
            if(srcUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcUser.c_str());
            }
            index += srcUser.size();

            //step14-3：软件标识长度
            tmpBuf[index] = srcVersion.size();
            index++;

            //step14-4：软件标识
            if(srcVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", srcVersion.c_str());
            }
            index += srcVersion.size();

            //step15：消息接收方
            //step15-1：用户长度

            tmpBuf[index] = dstUser.size();
            index++;

            //step15-2：用户字符串
            if(dstUser.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstUser.c_str());
            }
            index += dstUser.size();

            //step15-3：软件标识长度
            tmpBuf[index] = dstVersion.size();
            index++;

            //step15-4：软件标识
            if(dstVersion.size() != 0)
            {
                sprintf((char*)&tmpBuf[index], "%s", dstVersion.c_str());
            }
            index += dstVersion.size();

            //step16：消息通信信息字类型

            tmpBuf[index] = 0;
            index++;

            //step17：消息通信信息字个数
            byteSize = 2;
            data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, (int)wordMsg.size());
            index += byteSize;

            //step18：表号
            byteSize = 2;
            if(!jsValue["head_table"].isNull())
            {
                data.setData(tmpBuf, msgSize, index, byteSize, 0, 0, jsValue["head_table"].asInt());
            }
            index += byteSize;

            //step19：优先级
            tmpBuf[index] = 0x2;
            index++;

            //step20：备用字符串
            std::string reserved = "";
            if(!jsValue["head_reserve"].isNull())
            {
                reserved = jsValue["head_reserve"].asString();
            }
            //step20-1：长度
            tmpBuf[index] = reserved.size();
            index++;

            //step20-2：字符串
            if(reserved.size() > 0)
                memcpy(&tmpBuf[index], reserved.c_str(), reserved.size());

            index += reserved.size();

            //step21：信息字类型
            if(!jsValue["head_info_word_type"].isNull())
            {
                tmpBuf[index] = jsValue["head_info_word_type"].asInt();
            }
            index++;

            std::copy(tmpBuf, tmpBuf + index, std::back_inserter(outValue));

            //step22：追加信息字
            for(auto msg : wordMsg)
            {
                std::copy(msg.begin(), msg.end(), std::back_inserter(outValue));
            }

            //step23：更新长度+校验
            byteSize = 2;
            data.setData(&outValue.at(0), outValue.size(), 1, byteSize, 0, 0, (int)outValue.size());

            outValue[33] = Pf::PfCommon::Crc::calSum(&outValue.at(0), outValue.size());
        }

        void frameMiddle::_fillRegion(byteArray &outValue, const Json::Value &regionJs)
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
            paramsTable::getInstance()->getValues((unsigned int)tableNum, paramValues);

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
                    //modify xqx 20200423 当为字符串时，如果设置大小则按照设置填充，否则按照字符串大小进行填充
                    if(0 == byteSize)//按照字符串填充
                    {
                        memcpy(tmpBuf + startPos, initValue.c_str(), initValue.size());
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
                        memcpy(tmpBuf + startPos, initValue.c_str(), cpySize);
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

        bool frameMiddle::getAskMsg(const byteArray &inValue, byteArray &outValue, const Json::Value &json)
        {
            Json::Value headJs = json["head"];
            if(headJs.isNull() || headJs["head_is_ask"].isNull())
                return false;

            int tmpAskFalg = headJs["head_is_ask"].asInt();

            if(!( (0x1 == tmpAskFalg) || (0x3 == tmpAskFalg)))
                return false;

            dataStorage data;

            outValue.clear();
            std::copy(inValue.begin(), inValue.end(), std::back_inserter(outValue));

            //更新信源新宿
            unsigned char tmpV = 0;

            tmpV = outValue.at(4);
            outValue.at(4) = outValue.at(8);
            outValue.at(8) = tmpV;

            tmpV = outValue.at(5);
            outValue.at(5) = outValue.at(9);
            outValue.at(9) = tmpV;

            tmpV = outValue.at(6);
            outValue.at(6) = outValue.at(10);
            outValue.at(10) = tmpV;

            tmpV = outValue.at(7);
            outValue.at(7) = outValue.at(11);
            outValue.at(11) = tmpV;

            //更新消息出发点及消息目的地
            int pos = 0;
            int byteSize = 0;
            int len = 0;
            unsigned char send1Pos = 0, send1V = 0;
            unsigned char send2Pos = 0, send2V = 0;
            unsigned char send3Pos = 0, send3V = 0;
            unsigned char send4Pos = 0, send4V = 0;

            unsigned char rcv1Pos = 0, rcv1V = 0;
            unsigned char rcv2Pos = 0, rcv2V = 0;
            unsigned char rcv3Pos = 0, rcv3V = 0;
            unsigned char rcv4Pos = 0, rcv4V = 0;

            pos = 34;
            byteSize = 1;

            send1Pos = pos;
            send1V = outValue.at(pos);
            pos += byteSize;

            send2Pos = pos;
            send2V = outValue.at(pos);
            pos += byteSize;

            send3Pos = pos;
            send3V = outValue.at(pos);
            pos += byteSize;

            send4Pos = pos;
            send4V = outValue.at(pos);
            pos += byteSize;

            byteSize = 1;
            len = data.getData(&outValue.at(0), outValue.size(), pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            pos += byteSize;

            byteSize = 1;
            len = data.getData(&outValue.at(0), outValue.size(), pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            pos += byteSize;

            byteSize = 1;
            rcv1Pos = pos;
            rcv1V = outValue.at(pos);
            pos += byteSize;

            rcv2Pos = pos;
            rcv2V = outValue.at(pos);
            pos += byteSize;

            rcv3Pos = pos;
            rcv3V = outValue.at(pos);
            pos += byteSize;

            rcv4Pos = pos;
            rcv4V = outValue.at(pos);
            pos += byteSize;

            outValue.at(send1Pos) = rcv1V;
            outValue.at(send2Pos) = rcv2V;
            outValue.at(send3Pos) = rcv3V;
            outValue.at(send4Pos) = rcv4V;

            outValue.at(rcv1Pos) = send1V;
            outValue.at(rcv2Pos) = send2V;
            outValue.at(rcv3Pos) = send3V;
            outValue.at(rcv4Pos) = send4V;

            //step1：更新确认标志
            if(outValue.size() >= 24)
            {
                if(0x1 == tmpAskFalg)
                    outValue.at(24) = 0x2;
                else if(0x3 == tmpAskFalg)
                    outValue.at(24) = 0x4;
            }

            //step2：更新校验(先把校验位至0，然后再计算填充)
            if(outValue.size() >= 33)
            {
                outValue.at(33) = 0x0;

                unsigned short calCrc = PfCommon::Crc::calSum(&outValue.at(0), outValue.size());
                outValue.at(33) = calCrc & 0xFF;
            }

            return true;
        }

        void frameMiddle::frameCheck(unsigned char *u8Msg, const unsigned int u32Size)
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

        int frameMiddle::getInfoWordSize(unsigned char *inBuf, const unsigned int inSize)
        {
            dataStorage data;

            return data.getData(inBuf, inSize, getInfoWordSizeIndex(inBuf, inSize), 2, 0, 0);
        }

        int frameMiddle::getHeadSize(unsigned char *inBuf, const unsigned int inSize)
        {
            int index = getInfoWordIndex(inBuf, inSize);

            //信息字类型
            index += 1;

            return index;
        }

        int frameMiddle::getInfoWordSizeIndex(unsigned char *u8Msg, const unsigned int u32Size)
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

        int frameMiddle::getInfoWordIndex(unsigned char *u8Msg, const unsigned int u32Size)
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

            //step6：通用信息字个数
            index += 2;

            //step7：表号
            index += 2;

            //step8：优先级
            index += 1;

            //step9：备用字符串长度
            tmpLen = data.getData(u8Msg, u32Size, index, 1, 0, 0);
            index += 1;

            index += tmpLen;

            return index;
        }

        void frameMiddle::_parseHead(unsigned char *inBuf, const unsigned int inSize, Json::Value &headInfo)
        {
            dataStorage data;
            int len = 0;
            int pos = 0;
            int byteSize = 0;

            pos = 3;
            //step1：帧类型
            byteSize = 1;
            headInfo["head_frame_type"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step2：信源 系统类型、系统编码、节点编码
            byteSize = 1;
            headInfo["head_src_sys_type"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 2;
            headInfo["head_src_sys_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_src_node_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step3：信宿 系统类型、系统编码、节点编码
            byteSize = 1;
            headInfo["head_dst_sys_type"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 2;
            headInfo["head_dst_sys_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_dst_node_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step3：日期+时间
            byteSize = 2;
            headInfo["head_year"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_mon"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_day"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 4;
            headInfo["head_time"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step4：命令技术+确认标志、重传次数
            byteSize = 4;
            headInfo["head_cmd_cnt"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_is_ask"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_retry_cnt"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            pos += byteSize;

            byteSize = 2;
            pos += byteSize;

            byteSize = 4;
            pos += byteSize;

            byteSize = 1;
            pos += byteSize;

            //step1：消息出发点-用户字符串，软件标识

            byteSize = 1;
            headInfo["head_msg_out_sys_type"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 2;
            headInfo["head_msg_out_sys_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_msg_out_node_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_out_user"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_out_version"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;


            //step2：消息目的地用户字符串、软件标识

            byteSize = 1;
            headInfo["head_msg_in_sys_type"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 2;
            headInfo["head_msg_in_sys_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            headInfo["head_msg_in_node_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_in_user"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_in_version"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            //step3：消息发送方的地用户字符串、软件标识

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_send_user"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_send_version"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            //step4：消息接收送方的地用户字符串、软件标识

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_rcv_user"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_msg_rcv_version"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            //step5：消息类型（0：带表号的数据；1：自定义消息；2：日志消息）
            byteSize = 1;
            pos += byteSize;

            byteSize = 2;
            pos += byteSize;

            byteSize = 2;
            headInfo["head_table"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            byteSize = 1;
            pos += byteSize;

            //step7：备用字符串

            byteSize = 1;
            len = data.getData(inBuf, inSize, pos, 1, 0, 0);
            pos += byteSize;

            byteSize = len;
            if(pos + len <= inSize)
            {
                headInfo["head_reserve"] = std::string((const char*)&inBuf[pos], byteSize);
            }
            pos += byteSize;

            //step8：更新信息字类型
            byteSize = 1;
            headInfo["head_info_word_type"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);

        }

        bool frameMiddle::getInfoLen(unsigned char *inBuf, const unsigned int inSize, int infoType, int &len)
        {
            bool res = false;
            len = 0;

            if(Info_Word_0 == infoType)
            {
                //获取数据长度+备用长度
                dataStorage data;
                int pos = 6;

                int dataLen = data.getData(inBuf, inSize, pos, 1, 0, 0);
                pos += 1;
                pos += dataLen;

                int reserveLen = data.getData(inBuf, inSize, pos, 1, 0, 0);
                pos += 1;
                pos += reserveLen;

                len = pos;
            }
            else if(Info_Word_1 == infoType)
            {
                //获取数据长度+备用长度
                dataStorage data;
                int pos = 2;

                int dataLen = data.getData(inBuf, inSize, pos, 2, 0, 0);
                pos += 2;
                pos += dataLen;            

                int reserveLen = data.getData(inBuf, inSize, pos, 1, 0, 0);
                pos += 1;
                pos += reserveLen;

                len = pos;
            }
            else if(Info_Word_2 == infoType)
            {
                //获取数据长度+备用长度
                dataStorage data;
                int pos = 8;

                int dataLen = data.getData(inBuf, inSize, pos, 1, 0, 0);
                pos += 1;
                pos += dataLen;

                int reserveLen = data.getData(inBuf, inSize, pos, 1, 0, 0);
                pos += 1;
                pos += reserveLen;

                len = pos;
            }

            if( (len != 0) && (len <= inSize))
            {
                res = true;
            }

            return res;
        }

        void frameMiddle::_parseInfo(int infoType, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue, Json::Value &regionJs, int tableNum)
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

        void frameMiddle::_parseInfoWord0(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value)
        {
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            unDataConvert convertData;
            int dataType = 0;
            pos = 0;

            //step1：编码
            byteSize = 2;
            value["info_1_code"] = (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step2：数据类型
            byteSize = 1;
            value["info_1_data_type"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            dataType = value["info_1_data_type"].asInt();
            pos += byteSize;

            //step3：超差标记
            byteSize = 1;
            value["info_1_over"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step4：D编号
            byteSize = 2;
            value["info_1_num"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step5：数据长度
            byteSize = 1;
            value["info_1_data_len"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step6：数据
            byteSize = value["info_1_data_len"].asInt();

            value["info_1_src_data"] = QByteArray((const char*)&inBuf[pos], byteSize).toHex().toStdString();

            if(0 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_1_data"] = convertData.u32Value;                
            }
            else if(1 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_1_data"] = convertData.i32Value;
            }
            else if(2 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_1_data"] = convertData.f32Value;
            }
            else if(3 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_1_data"] = convertData.d64Value;
            }
            else if(4 == dataType)
            {
                value["info_1_data"] = std::string((const char*)&inBuf[pos], byteSize);
            }

            pos += byteSize;

            //step7：备用长度
            byteSize = 1;
            value["info_1_reserve_len"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step8：备用字符串
            byteSize = value["info_1_reserve_len"].asInt();
            if(byteSize > 0)
            {
                value["info_1_reserve"] = std::string((const char*)&inBuf[pos], byteSize);
            }
        }

        void frameMiddle::_parseInfoWord1(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value, Json::Value &regionJs, int tableNum)
        {
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            unDataConvert convertData;
            int dataType = 0;
            pos = 0;

            //step1：编码
            byteSize = 2;
            value["info_2_code"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step2：数据长度
            byteSize = 2;
            value["info_2_data_len"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step3：数据
            byteSize = value["info_2_data_len"].asInt();

            //step4：解析二进制数据
            _parseRegion(tableNum, &inBuf[pos], byteSize, regionJs);

            if(tableNum == 4283)
                return ;

            pos += byteSize;

            //step5：备用长度
            byteSize = 1;
            value["info_2_reserve_len"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step6：备用字符串
            byteSize = value["info_2_reserve_len"].asInt();
            if(byteSize > 0)
            {
                value["info_2_reserve"] = std::string((const char*)&inBuf[pos], byteSize);
            }
        }

        void frameMiddle::_parseInfoWord2(const unsigned char *inBuf, const unsigned int inSize, Json::Value &value)
        {
            int pos = 0;
            int byteSize = 0;
            dataStorage data;
            unDataConvert convertData;
            int dataType = 0;
            pos = 0;

            //step1：编码
            byteSize = 2;
            value["info_3_code"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step2：设备序号
            byteSize = 1;
            value["info_3_dev_num"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step3：模块序号
            byteSize = 1;
            value["info_3_module"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step4：数据类型
            byteSize = 1;
            value["info_3_data_type"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            dataType = value["info_3_data_type"].asInt();
            pos += byteSize;

            //step5：超差标记
            byteSize = 1;
            value["info_3_over"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step6：D编号
            byteSize = 2;
            value["info_3_num"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step7：数据长度
            byteSize = 1;
            value["info_3_data_len"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step8：数据
            byteSize = value["info_3_data_len"].asInt();

            value["info_3_src_data"] = QByteArray((const char*)&inBuf[pos], byteSize).toHex().toStdString();

            if(0 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_3_data"] = convertData.u32Value;
            }
            else if(1 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_3_data"] = convertData.i32Value;
            }
            else if(2 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_3_data"] = convertData.f32Value;
            }
            else if(3 == dataType)
            {
                convertData = data.getAutoData(inBuf, inSize, pos, byteSize, 0, 0);
                value["info_3_data"] = convertData.d64Value;
            }
            else if(4 == dataType)
            {
                value["info_3_data"] = std::string((const char*)&inBuf[pos], byteSize);
            }

            pos += byteSize;

            //step9：备用长度
            byteSize = 1;
            value["info_3_reserve_len"] =  (int)data.getData(inBuf, inSize, pos, byteSize, 0, 0);
            pos += byteSize;

            //step10：备用字符串
            byteSize = value["info_3_reserve_len"].asInt();
            if(byteSize > 0)
            {
                value["info_3_reserve"] = std::string((const char*)&inBuf[pos], byteSize);
            }
        }


        bool frameMiddle::getValidValue(const Json::Value &result, Json::Value &value, int &infoType)
        {
            bool res = false;

            //获取表号+编码
            //获取信息字类型
            if(!result["head"]["head_info_word_type"].isNull())
            {
                int type = result["head"]["head_info_word_type"].asInt();
                infoType = type;
                int tableNum = result["head"]["head_table"].asInt();
                std::string codeKey = "info_" + std::to_string(type + 1) + "_code";
                std::string tableKey = "info_" + std::to_string(type + 1) + "_table_num";
                std::string valueKey = "info_" + std::to_string(type + 1) + "_data";
                std::string srcValueKey = "info_" + std::to_string(type + 1) + "_src_data";

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

                        if(!infoWord[index][valueKey].isNull())
                        {
                            tmpJs["value"] = infoWord[index][valueKey].asInt();
                        }

                        if(!infoWord[index][srcValueKey].isNull())
                        {
                            tmpJs["src_value"] = infoWord[index][srcValueKey].asString();
                        }

                        value.append(tmpJs);
                    }
                }
            }

            return !value.isNull();
        }

        bool frameMiddle::parse(unsigned char *u8Msg, const unsigned int u32Size, Json::Value &result)
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
            unsigned int infoCnt = getInfoWordSize(u8Msg, u32Size);

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
//#ifndef QT_NO_DEBUG
//                    std::cout << " -> ";
//                    for(int index = 0; index < tmpLen; index++)
//                    {
//                        std::cout << std::hex << (int)u8Msg[msgPos + index] << " ";
//                    }
//                    std::cout << std::endl;
//#endif
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

        void frameMiddle::_parseRegion(const int &tableNum, const unsigned char *u8Msg, const unsigned int u32Size, Json::Value &regionValue)
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
                    //modify xqx 2020-6-28 09:28:17 字符串长度大于总长度时按照最小获取（字符串异常），软件不至于崩溃

                    preValue > (u32Size - startPos) ? preValue = u32Size - startPos : preValue;

                    //end
                    std::string calResult = std::string((const char*)&u8Msg[startPos], preValue);

                    tmpValue["src_value"] = QByteArray((const char*)&u8Msg[startPos], preValue).toHex().toStdString();

                    tmpValue["value"] = calResult;

                    preStartPos = startPos + preValue;
                }
                else
                {
                    unDataConvert pValue = data.getAutoData(u8Msg, u32Size, startPos, byteSize, bitPos, bitSize, bigSmall);

                    tmpValue["src_value"] = QByteArray((const char*)&u8Msg[startPos], byteSize).toHex().toStdString();

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
                return new frameMiddle();
            }
        }
    }
}
