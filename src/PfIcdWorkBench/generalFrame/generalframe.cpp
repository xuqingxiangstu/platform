#include "generalframe.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../../PfCommon/crc/crc.h"

#include "../common/subProtocol.h"

#include "../icdData/datastorage.h"
#include "../icdData/dataFrame.h"
#include "../icdData/datacalc.h"
#include "../icdData/dataconvert.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        generalFrame::generalFrame()
            :mFrameCfgPath(""),
             mDataRegionCfgPath("")
        {

        }
        generalFrame::~generalFrame()
        {

        }

        void generalFrame::parse(const unsigned char *u8Msg, const unsigned int u32Size, std::vector<icdOutConvertValueType> &convertOutValue)
        {

            std::ostringstream strErr;

            try
            {
                /// 帧校验
                frameCheck(u8Msg, u32Size);

                dataStorage data;
                /// 信息字类型
                unsigned int frameCode = data.getData(u8Msg, u32Size,
                                                      mProtocolCfg->getMessage<protocolConfigure::general_info_type_start_index>(),
                                                      mProtocolCfg->getMessage<protocolConfigure::general_info_type_size_index>(),
                                                      0, 0);

       #if 0
                if(!mSubProtocolCfg->isExist(frameCode))
                {
                    strErr << "帧识别码(" << std::hex << frameCode << ")，不存在";
                    UT_THROW_EXCEPTION(strErr.str());
                }

                std::vector<std::shared_ptr<subProtocolConfigure::subStorageType>> subStorages;
                if(!mSubProtocolCfg->getStorages(subStorages, frameCode))
                {
                    strErr << "获取子帧信息失败，帧识别码(" << std::hex << frameCode << ")";
                    UT_THROW_EXCEPTION(strErr.str());
                }

                int beyond = mSubProtocolCfg->getMsgBeyond(frameCode);

                dataFrame fParse;
                fParse.parse(subStorages, convertOutValue, u8Msg, u32Size, beyond);
#endif
            }
            catch(std::runtime_error err)
            {
                std::string errCode = err.what();
                UT_THROW_EXCEPTION(errCode);
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

        void generalFrame::simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode, const std::vector<icdInValueType> inValue)
        {
#if 0
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
#endif
        }

        void generalFrame::_simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode, const std::vector<icdInValueType> inValue)
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

            //obj->mProtocolCfg = this->mProtocolCfg->clone();
            //obj->mSubProtocolCfg = this->mSubProtocolCfg->clone();

            obj->mFrameCfgPath = this->mFrameCfgPath;
            obj->mDataRegionCfgPath = this->mDataRegionCfgPath;

            std::shared_ptr<frameObj> tmp(obj);

            return tmp;
        }

        void generalFrame::frameCheck(const unsigned char *u8Msg, const unsigned int u32Size)
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
                crc = PfCommon::Crc::calSum((unsigned char*)(&u8Msg[checkStartPos]), checkSize);
                //modify xqx 20200211
                crc &= 0xF;
            }


            unsigned int calCrc = data.getData(u8Msg, u32Size,
                                               mProtocolCfg->getMessage<protocolConfigure::general_frame_check_start_index>(),
                                               mProtocolCfg->getMessage<protocolConfigure::general_frame_check_size_index>(), 0, 0);

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

        }

        void generalFrame::initSubFrameCfg(const std::string &path)
        {
#if 0
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

            mSubProtocolCfg = std::make_shared<subProtocolConfigure>();
            mSubProtocolCfg->init(workBook);
#endif
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
