#include "variableframe.h"

#include "../../PfCommon/tools/ut_error.h"
#include "../../PfCommon/crc/crc.h"
#include "../icdData/datastorage.h"
#include "../icdData/dataFrame.h"

namespace Pf
{
    namespace PfIcdWorkBench
    {
        variableFrame::variableFrame()
            :mFrameCfgPath(""),
              mDataRegionCfgPath("")
        {

        }

        variableFrame::~variableFrame()
        {

        }

        void variableFrame::init(const TiXmlElement *xmlEle)
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

        void variableFrame::simulation(byteArray &outValue, const unsigned int frameCode, const unsigned int insideCode, const std::vector<icdInValueType> inValue)
        {

        }

        void variableFrame::parse(const unsigned char *u8Msg, const unsigned int u32Size, std::vector<icdOutSrcValueType> &outValue)
        {
            std::ostringstream strErr;

            try
            {
                /// 帧校验
                frameCheck(u8Msg, u32Size);

                dataStorage data;
                /// 获取子帧识别码
                unsigned int frameCode = data.getData(u8Msg, u32Size,
                                                      mProtocolCfg->getMessage<protocolConfigure::variable_code_start_index>(),
                                                      mProtocolCfg->getMessage<protocolConfigure::variable_code_size_index>(),
                                                      mProtocolCfg->getMessage<protocolConfigure::variable_code_bit_start_index>(),
                                                      mProtocolCfg->getMessage<protocolConfigure::variable_code_bit_size_index>());

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

                // 子帧解析
                dataFrame fParse;
                fParse.parse(subStorages, outValue, u8Msg, u32Size, beyond);
            }
            catch(std::runtime_error err)
            {
                std::string errCode = err.what();
                UT_THROW_EXCEPTION(errCode);
            }
        }

        void variableFrame::frameCheck(const unsigned char *u8Msg, const unsigned int u32Size)
        {
            std::ostringstream strErr;
            int i32Pos = 0;
            unDataConvert mData;
            mData.u64Value = 0;
            dataStorage data;

            //帧同步码
            int headSize = mProtocolCfg->getMessage<protocolConfigure::variable_head_size_index>();

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

            if (mData.u32Value != mProtocolCfg->getMessage<protocolConfigure::variable_head_init_index>())
            {
                strErr.str("");
                strErr << "帧同步码校验错误, 接收0x" << std::hex << mData.u32Value << " 实际0x" << mProtocolCfg->getMessage<protocolConfigure::variable_head_init_index>();
                throw std::runtime_error(strErr.str());
            }
            //帧长度校验
            unsigned int u32GetSize = data.getData(u8Msg, u32Size, mProtocolCfg->getMessage<protocolConfigure::variable_frame_len_start_index>(),
                                                   mProtocolCfg->getMessage<protocolConfigure::variable_frame_len_byte_size_index>(), 0, 0);

            u32GetSize = u32GetSize + mProtocolCfg->getMessage<protocolConfigure::variable_cal_len_start_index>() + mProtocolCfg->getMessage<protocolConfigure::variable_cal_len_to_end_index>();

            if (u32Size != u32GetSize)
            {
                strErr.str("");
                strErr << "帧长度校验错误, 接收" << std::dec << u32Size << " 实际" << u32GetSize;
                UT_THROW_EXCEPTION(strErr.str());
            }

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
            }


            unsigned int calCrc = data.getData(u8Msg, u32Size,
                                               u32Size - mProtocolCfg->getMessage<protocolConfigure::variable_frame_check_to_end_index>(),
                                               mProtocolCfg->getMessage<protocolConfigure::variable_frame_check_size_index>(), 0, 0);

            if (calCrc != crc)
            {
                strErr.str("");
                strErr << "帧校验和校验错误, 接收0x" << std::hex << calCrc << " 实际计算0x" << crc;
                UT_THROW_EXCEPTION(strErr.str());
            }
        }

        void variableFrame::initFrameCfg(const std::string &path)
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

            mProtocolCfg = std::make_shared<protocolConfigure>();
            mProtocolCfg->init(static_cast<QXlsx::Worksheet*>(workBook->sheet(0)));
        }

        void variableFrame::initSubFrameCfg(const std::string &path)
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

            mSubProtocolCfg = std::make_shared<subProtocolConfigure>();
            mSubProtocolCfg->init(workBook);
        }

        extern "C"
        {
            frameObj *LoadClass()
            {
                return new variableFrame();
            }
        }

    }
}
