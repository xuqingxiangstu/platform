#include "m1553b.h"

#include <sstream>
#include <thread>
#include <exception>
#include <string.h>

namespace Pf
{
    namespace PfBus
    {
        M1553B::M1553B()
#ifndef NO_1553B_CARD
            :mDevHandle(0),
            mChannel(0),
            mServerAddr("192.168.1.61"),
            mServerPort(15530)
#endif
        {
        }

        M1553B::~M1553B()
        {
#ifndef NO_1553B_CARD
            if(mDevHandle)
            {
                MF1553_Close(mDevHandle);
                mDevHandle = 0;
            }
#endif
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

        void M1553B::Rt2BcMsg(int rtAddr, int saAddr, int msgSize, int bus)
        {
#ifndef NO_1553B_CARD
            STRUCT_ZHHK1553_BC_SMFRM BcSendMajorFrame;

            // BC主帧中微帧数量
            BcSendMajorFrame.SFrmCnt = 1;

            /* 微帧 0: ===================================================== */
                /*
                ① 微帧执行状态：1          // 立即执行
                ② 微帧运行周期：0          // 事件微帧
                ③ 微帧时间间隔：0          //
                ④ 微帧重发次数：0          //
                ⑤ 微帧消息数量：2047       //
                ⑥ 微帧附加功能：无         //
                ⑦ 消息组中，BC向RT1的0~30子地址轮询发送顺序数
                */
            int frmid = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmRun = 1;
            BcSendMajorFrame.SFrm[frmid].SFrmPeriod = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmGapTime = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmRepeatCnt = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsgCnt = 1;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeEndTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeStartTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeEndTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeStartTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeRxTriggerDivCnt = 0;

            int msgid = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgChannel = bus;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgFormat = BC_2_RT;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgRetryEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgGapTime = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeEndTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeStartTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeEndTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeStartTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeOkMsgRcvXEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeErrMsgRcvXEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchToFrmId = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataLocation = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataMask = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataExpectation = 0;

            // 配置命令字
            int tnr         = 1;
            int count       = msgSize;

            WORD cmd = ((rtAddr & 0x001f) << 11) | ((tnr & 0x0001) << 10) | ((saAddr & 0x001f) << 5) | (count & 0x001f);

            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBlock[0] = cmd;

            MF1553_BC_WriteFrm(mDevHandle, mChannel, frmid, &BcSendMajorFrame.SFrm[frmid]);    // 写入微帧
#if 1
            MF1553_BC_AddEndOfFrmList(mDevHandle, mChannel, BcSendMajorFrame.SFrmCnt);
            MF1553_BC_MsgBufferClear(mDevHandle, mChannel);
            MF1553_BC_Start(mDevHandle, mChannel);
            //std::this_thread::sleep_for(std::chrono::microseconds(20));
            MF1553_BC_Stop(mDevHandle, mChannel);
#endif
#endif
        }

        bool M1553B::Bc2RtMsg(unsigned short *u8Msg, unsigned int u32MsgLen, int rtAddr, int saAddr, int bus)
        {
#ifndef NO_1553B_CARD
            STRUCT_ZHHK1553_BC_SMFRM BcSendMajorFrame;

            // BC主帧中微帧数量
            BcSendMajorFrame.SFrmCnt = 1;

            /* 微帧 0: ===================================================== */
                /*
                ① 微帧执行状态：1          // 立即执行
                ② 微帧运行周期：0          // 事件微帧
                ③ 微帧时间间隔：0          //
                ④ 微帧重发次数：0          //
                ⑤ 微帧消息数量：2047       //
                ⑥ 微帧附加功能：无         //
                ⑦ 消息组中，BC向RT1的0~30子地址轮询发送顺序数
                */
            int frmid = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmRun = 1;
            BcSendMajorFrame.SFrm[frmid].SFrmPeriod = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmGapTime = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmRepeatCnt = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsgCnt = 1;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeEndTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeStartTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeEndTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeStartTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeRxTriggerDivCnt = 0;

            int msgid = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgChannel = bus;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgFormat = BC_2_RT;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgRetryEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgGapTime = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeEndTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeStartTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeEndTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeStartTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeOkMsgRcvXEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeErrMsgRcvXEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchToFrmId = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataLocation = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataMask = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataExpectation = 0;

            // 配置命令字
            int tnr         = 0;
            int count       = u32MsgLen;

            WORD cmd = ((rtAddr & 0x001f) << 11) | ((tnr & 0x0001) << 10) | ((saAddr & 0x001f) << 5) | (count & 0x001f);

            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBlock[0] = cmd;
            // 配置数据
            for (int msgcnt=0; msgcnt < u32MsgLen; msgcnt++)
            {
                BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBlock[msgcnt+1] = u8Msg[msgcnt]; // 数据
            }


            MF1553_BC_WriteFrm(mDevHandle, mChannel, frmid, &BcSendMajorFrame.SFrm[frmid]);    // 写入微帧
#if 1
            MF1553_BC_AddEndOfFrmList(mDevHandle, mChannel, BcSendMajorFrame.SFrmCnt);
            MF1553_BC_MsgBufferClear(mDevHandle, mChannel);
            MF1553_BC_Start(mDevHandle, mChannel);
            //std::this_thread::sleep_for(std::chrono::microseconds(20));
            MF1553_BC_Stop(mDevHandle, mChannel);
#endif
#endif
            return true;
        }

        bool M1553B::Rt2RtMsg(int sRtAddr, int sSaAddr, int rRtAddr, int rSaAddr, int msgSize, int bus)
        {
#ifndef NO_1553B_CARD
            STRUCT_ZHHK1553_BC_SMFRM BcSendMajorFrame;

            // BC主帧中微帧数量
            BcSendMajorFrame.SFrmCnt = 1;

            /* 微帧 0: ===================================================== */
                /*
                ① 微帧执行状态：1          // 立即执行
                ② 微帧运行周期：0          // 事件微帧
                ③ 微帧时间间隔：0          //
                ④ 微帧重发次数：0          //
                ⑤ 微帧消息数量：2047       //
                ⑥ 微帧附加功能：无         //
                ⑦ 消息组中，BC向RT1的0~30子地址轮询发送顺序数
                */
            int frmid = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmRun = 1;
            BcSendMajorFrame.SFrm[frmid].SFrmPeriod = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmGapTime = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmRepeatCnt = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsgCnt = 1;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeEndTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeStartTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeEndTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeStartTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmMode.SFrmModeRxTriggerDivCnt = 0;

            int msgid = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgChannel = bus;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgFormat = RT_2_RT;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgRetryEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgGapTime = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeEndTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeStartTrgOutEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeEndTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeStartTrgIntEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeOkMsgRcvXEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgMode.SMsgModeErrMsgRcvXEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchEn = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchToFrmId = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataLocation = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataMask = 0;
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBranch.SMsgBranchDataExpectation = 0;

            // 配置接收命令字
            int tnr = 0;
            WORD cmd = 0;

            cmd = ((rRtAddr & 0x001f) << 11) | ((tnr & 0x0001) << 10) | ((rSaAddr & 0x001f) << 5) | (msgSize & 0x001f);
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBlock[0] = cmd;

            // 配置发送命令字
            tnr         = 1;
            cmd = ((sRtAddr & 0x001f) << 11) | ((tnr & 0x0001)<<10) | ((sSaAddr & 0x001f) << 5) | (msgSize & 0x001f);
            BcSendMajorFrame.SFrm[frmid].SFrmSMsg[msgid].SMsgBlock[1] = cmd;

            MF1553_BC_WriteFrm(mDevHandle, mChannel, frmid, &BcSendMajorFrame.SFrm[frmid]);    // 写入微帧
            MF1553_BC_AddEndOfFrmList(mDevHandle, mChannel, BcSendMajorFrame.SFrmCnt);
            MF1553_BC_MsgBufferClear(mDevHandle, mChannel);
            MF1553_BC_Start(mDevHandle, mChannel);
            //std::this_thread::sleep_for(std::chrono::microseconds(20));
            MF1553_BC_Stop(mDevHandle, mChannel);

#endif

            return true;
        }

        bool M1553B::receiveMsg(unsigned short &rt, unsigned short &sa, unsigned short *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {
            bool res = false;

#ifndef NO_1553B_CARD
            DWORD MsgNum = 0;
            DWORD FrmIds[512] = {0};
            DWORD MsgIds[512] = {0};

            //请求RT->BC消息
            Rt2BcMsg(3, 8, 6, Bus_A);

            MF1553_BC_GetMsgNumNewly(mDevHandle, mChannel, &MsgNum);
            if (MsgNum !=0 )
            {
                MF1553_BC_ReadMsgs(mDevHandle, mChannel, MsgNum, FrmIds, MsgIds, mRcvMsg);

                for(int index = 0; index < MsgNum; index++)
                {
                    WORD int_sts = mRcvMsg[index].RMsgBlock[0];

                    if (((mRcvMsg[index].RMsgBlockStatus) & BCMS_CH_BORA) == BCMS_CH_BORA)
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

                    if(0 == Tr)
                        continue;

                    int msgSize = int_sts & 0x1F;

                    if(0 == msgSize)
                        msgSize = 32;

                    memcpy(u8Msg, &mRcvMsg[index].RMsgBlock[2], msgSize * sizeof(WORD));

                    *u32MsgLen = msgSize;

                    res = true;
                }
            }
#else
            return false;
#endif
            return res;
        }
    }
}
