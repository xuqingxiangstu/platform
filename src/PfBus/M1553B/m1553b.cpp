#include "m1553b.h"

#include <sstream>

#include <exception>

#include "./ZHHK1553/include/defines.h"

namespace Pf
{
    namespace PfBus
    {
        M1553B::M1553B():
            mDevHandle(0),
            mChannel(0)
        {
        }

        M1553B::~M1553B()
        {

        }

        void M1553B::init(std::string cardNum, std::string mode)
        {            
            std::ostringstream strErr;
#ifndef NO_1553B_CARD
            //打开板卡
            if (MF1553_Open(&mDevHandle, (char*)mServerAddr.c_str(), mServerPort) != 0)
            {
                mDevHandle = 0;
                strErr << "打开板卡失败!";
                throw std::runtime_error(strErr.str());
            }

            // 全局复位
            if (MF1553_Reset(mDevHandle) != 0)
            {
                strErr << "复位板卡失败!";
                throw std::runtime_error(strErr.str());
            }

            // 通道复位
            if (MF1553_ResetChannel(mDevHandle, mChannel) != 0)
            {
                strErr << "复位板卡通道失败!";
                throw std::runtime_error(strErr.str());
            }

            // ================配置板卡================
            // 板卡速率选择
            MF1553_SpeedSel(mDevHandle, mChannel, 0); // 0：1Mbps; 1：4Mbps
            // 设置时标使能
            MF1553_AddTimeTag(mDevHandle, mChannel, TRUE);
            // 设置响应超时时间 = 24 * 0.5us = 12us
            MF1553_SetResponseTimeout(mDevHandle, mChannel, 24);

            // ================BC初始化================
            MF1553_BC_Init (mDevHandle, mChannel);

            // ================BC各种配置==============
            // 配置BC消息重试功能
            STRUCT_ZHHK1553_BC_RETRY_CASE BcRetryCase;
            BcRetryCase.RetryEn = 0;
            BcRetryCase.RetryNum = 0;
            BcRetryCase.RetryIfMsgError = 0;
            BcRetryCase.RetryIfStatusSet = 0;
            BcRetryCase.RetryAlterChannelOnBus1 = 0;
            BcRetryCase.RetryAlterChannelOnBus2 = 0;

            // 配置BC消息/帧停止功能
            STRUCT_ZHHK1553_BC_STOP_CASE BcStopCase;
            BcStopCase.MsgStopOnStatusSet = 0;
            BcStopCase.MsgStopOnMsgError = 0;
            BcStopCase.FrmStopOnStatusSet = 0;
            BcStopCase.FrmStopOnMsgError = 0;
            // 配置BC
            MF1553_BC_CfgWord(mDevHandle, mChannel, &BcRetryCase, &BcStopCase);
#endif
        }

        bool M1553B::sendMsg(unsigned short *u8Msg, unsigned int u32MsgLen, int rtAddr, int saAddr)
        {
            int frameId = 0;
            STRUCT_ZHHK1553_BC_SMFRM sendMajorFrame;
            sendMajorFrame.SFrm[frameId].SFrmRun = 1;
            sendMajorFrame.SFrm[frameId].SFrmPeriod = 0;
            sendMajorFrame.SFrm[frameId].SFrmGapTime = 0;
            sendMajorFrame.SFrm[frameId].SFrmRepeatCnt = 0;
            sendMajorFrame.SFrm[frameId].SFrmSMsgCnt = 1;
            sendMajorFrame.SFrm[frameId].SFrmMode.SFrmModeEndTrgOutEn = 0;
            sendMajorFrame.SFrm[frameId].SFrmMode.SFrmModeStartTrgOutEn = 0;
            sendMajorFrame.SFrm[frameId].SFrmMode.SFrmModeEndTrgIntEn = 0;
            sendMajorFrame.SFrm[frameId].SFrmMode.SFrmModeStartTrgIntEn = 0;
            sendMajorFrame.SFrm[frameId].SFrmMode.SFrmModeRxTriggerDivCnt = 0;

            // 配置命令字
            int tnr         = 0;
            int count       = u32MsgLen;

            WORD cmd = ((rtAddr&0x001f) << 11) | ((tnr&0x0001)<<10) | ((saAddr&0x001f) << 5) | (count&0x001f);
            sendMajorFrame.SFrm[frameId].SFrmSMsg[0].SMsgBlock[0] = cmd;
            // 配置数据
            for (int msgcnt=0; msgcnt < u32MsgLen; msgcnt++)
            {
                sendMajorFrame.SFrm[frameId].SFrmSMsg[0].SMsgBlock[msgcnt+1] = u8Msg[msgcnt]; // 数据
            }
#ifndef NO_1553B_CARD
            MF1553_BC_WriteFrm(mDevHandle, mChannel, frameId, &sendMajorFrame.SFrm[0]);    // 写入微帧
            MF1553_BC_AddEndOfFrmList(mDevHandle, mChannel, sendMajorFrame.SFrmCnt);
            MF1553_BC_MsgBufferClear(mDevHandle, mChannel);
            MF1553_BC_Start(mDevHandle, mChannel);
#endif
            return true;
        }

        bool M1553B::receiveMsg(unsigned short &rt, unsigned short &sa, unsigned short *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {
            DWORD MsgNum = 0;
            DWORD FrmIds[32768] = {0};
            DWORD MsgIds[32768] = {0};
            STRUCT_ZHHK1553_RMSG receiveMsg;

            memset(&receiveMsg, 0, sizeof(STRUCT_ZHHK1553_RMSG));
#ifndef NO_1553B_CARD
            MF1553_BC_GetMsgNumNewly(mDevHandle, mChannel, &MsgNum);
            if (MsgNum !=0 )
            {
                MF1553_BC_ReadMsgs(mDevHandle, mChannel, MsgNum, FrmIds, MsgIds, &receiveMsg);

                WORD int_sts = receiveMsg.RMsgBlock[0];

                if (((receiveMsg.RMsgBlockStatus) & BCMS_CH_BORA) == BCMS_CH_BORA)
                {
                    //channel b
                }
                else
                {
                    //channel a
                }

                rt = ((int_sts & 0xF800) >> 11);
                sa = ((int_sts & 0x03E0) >> 5);

                //1->发送 0->接收
                int Tr = (int_sts & 0x400) >> 10;

                if(1 == Tr)
                    return false;

                int msgSize = int_sts & 0x1F;

                if(msgSize = 0)
                    msgSize = 32;

                memcpy(u8Msg, &receiveMsg.RMsgBlock[2], msgSize * sizeof(WORD));

                *u32MsgLen = msgSize;
            }
#else
            return false;
#endif
            return true;
        }
    }
}
