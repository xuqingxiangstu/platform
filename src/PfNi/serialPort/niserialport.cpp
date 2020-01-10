#include "niserialport.h"

#include <sstream>
#include <stdexcept>

namespace Pf
{
    namespace PfNi
    {
        NiSerialPort::NiSerialPort()
        {

        }

        NiSerialPort::~NiSerialPort()
        {

        }

        void NiSerialPort::init(const std::string &rName, const std::string &parity, const int &baud, const int &dataBits, const std::string stopBits)
        {
            std::ostringstream strErr;
            ViStatus nReturnS = 0;

            nReturnS = viOpenDefaultRM(&mSession);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << rName << ")viOpenDefaultRM ";
                throw std::runtime_error(strErr.str());
            }
            nReturnS = viOpen(mSession,ViRsrc(rName.c_str()),VI_NULL,VI_NULL,&mInstrHandle);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << rName << ")viOpen ";
                throw std::runtime_error(strErr.str());
            }

            nReturnS = viSetAttribute(mInstrHandle, VI_ATTR_ASRL_BAUD, baud);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << baud << ")viSetAttribute(VI_ATTR_ASRL_BAUD) ";
                throw std::runtime_error(strErr.str());
            }

            nReturnS = viSetAttribute(mInstrHandle, VI_ATTR_ASRL_DATA_BITS, dataBits);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << dataBits << ")viSetAttribute(VI_ATTR_ASRL_DATA_BITS) ";
                throw std::runtime_error(strErr.str());
            }

            ViAttrState Value = getParityV(parity);

            nReturnS = viSetAttribute(mInstrHandle, VI_ATTR_ASRL_PARITY, Value);

            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << Value << ")viSetAttribute(VI_ATTR_ASRL_PARITY) ";
                throw std::runtime_error(strErr.str());
            }

            Value = getStopBitsV(stopBits);

            nReturnS = viSetAttribute(mInstrHandle, VI_ATTR_ASRL_STOP_BITS, Value);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << stopBits << ")viSetAttribute(VI_ATTR_ASRL_STOP_BITS) ";
                throw std::runtime_error(strErr.str());
            }

            //设置超时时间
            nReturnS = viSetAttribute(mInstrHandle, VI_ATTR_TMO_VALUE, 2000);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << 2000 << ")viSetAttribute(VI_ATTR_TMO_VALUE) ";
                throw std::runtime_error(strErr.str());
            }

            //终止符
            nReturnS = viSetAttribute(mInstrHandle, VI_ATTR_ASRL_END_OUT, VI_ASRL_END_NONE);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR] viSetAttribute(VI_ATTR_SEND_END_EN) ";
                throw std::runtime_error(strErr.str());
            }

            nReturnS = viSetAttribute(mInstrHandle, VI_ATTR_ASRL_END_IN, VI_ASRL_END_NONE);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR]viSetAttribute(VI_ATTR_SEND_END_EN) ";
                throw std::runtime_error(strErr.str());
            }

            //设置收发缓冲区大小
            // 发送
            nReturnS = viSetBuf(mInstrHandle, VI_IO_OUT_BUF, 1024);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" <<rName << ")viSetBuf(VI_IO_OUT_BUF) ";
                throw std::runtime_error(strErr.str());
            }
            //接收
            nReturnS = viSetBuf(mInstrHandle, VI_IO_IN_BUF, 1024);
            if(nReturnS != VI_SUCCESS)
            {
                strErr << "[ERROR](" << rName << ")viSetBuf(VI_ATTR_ASRL_STOP_BITS) ";
                throw std::runtime_error(strErr.str());
            }
        }
        ViAttrState NiSerialPort::getStopBitsV(const std::string &StopBits)
        {
            ViAttrState Value = 0;

            if (StopBits == "1")
            {
                Value = VI_ASRL_STOP_ONE;
            }
            else if (StopBits == "1.5")
            {
                Value = VI_ASRL_STOP_ONE5;
            }
            else if (StopBits == "2")
            {
                Value = VI_ASRL_STOP_TWO;
            }

            return Value;
        }

        ViAttrState NiSerialPort::getParityV(const std::string &mParity)
        {
            ViAttrState Value;

            if(mParity == "NONE")
            {
                Value = VI_ASRL_PAR_NONE;
            }
            else if(mParity == "EVEN")
            {
                Value = VI_ASRL_PAR_EVEN;
            }
            else if(mParity == "MARK")
            {
                Value = VI_ASRL_PAR_MARK;
            }
            else if(mParity == "SPACE")
            {
                Value = VI_ASRL_PAR_SPACE;
            }
            else if(mParity == "ODD")
            {
                Value = VI_ASRL_PAR_ODD;
            }
            else
            {
                Value = VI_ASRL_PAR_NONE;
            }

            return Value;
        }

        bool NiSerialPort::sendMsg(unsigned char *u8Msg, const unsigned int &u32MsgLen)
        {
            bool Res = false;

            ViUInt32 retCount = VI_NULL;

            if (VI_SUCCESS == viWrite(mInstrHandle, ViBuf(u8Msg), ViUInt32(u32MsgLen), &retCount))
            {
                if ((retCount) == u32MsgLen)
                {
                    Res = true;
                }

            }

            return Res;
        }

        bool NiSerialPort::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen)
        {
            ViUInt32 retCount = VI_NULL;
            bool Res = false;

            ViUInt32 count = 0;
            viGetAttribute(mInstrHandle, VI_ATTR_ASRL_AVAIL_NUM, &count);
            viRead(mInstrHandle, ViPBuf(u8Msg), count, &retCount);

            if(retCount > 0)
            {
                *u32MsgLen = retCount;

                Res = true;
            }

            return Res;
        }
    }
}
