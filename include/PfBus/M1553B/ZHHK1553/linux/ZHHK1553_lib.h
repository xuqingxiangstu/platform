//=====================================================================
//--------------File - ZHHK1553_lib.h ------------------
//
//-----Library for accessing the ZHHK1553-ETH Series card-----
//   Copyright (C) ZhengHong Aviation Tech Co., Ltd; 
//   http://www.zhhktech.com; 029-84288198
//   DATE:2019-06-17  V13.1 Series
// 
//=====================================================================

#ifndef _ZHHK1553_LIB_H_
#define _ZHHK1553_LIB_H_

#include "os.h"
#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================= V12 Add Start
/*
typedef void* DevInfoHandle;    // 设备信息句柄
typedef DWORD DevHandle;        // 设备句柄
typedef void (__stdcall *ERROR_HANDLER)(PVOID pDevDesc, int ErrorCode);
*/
typedef int DevHandle;
typedef int HANDLE;

#define API
#define VAL_SUCCESS   0
#define VAL_TRUE      1
#define VAL_FALSE     2
#define VAL_FAILED   -1
#define VAL_PARERR   -2
#define VAL_TIME_OUT -5
#define VAL_UNKNOWN_STATUS -4
#define ErrorDeviceHandleInvalid -1000
#define ErrorFindConfigurationSpace -1001
// ========================================================= V12 Add End

// type defines
typedef struct
{
    WORD  SMsgModeEndTrgOutEn;
    WORD  SMsgModeStartTrgOutEn;
    WORD  SMsgModeEndTrgIntEn;
    WORD  SMsgModeStartTrgIntEn;
    WORD  SMsgModeOkMsgRcvXEn;
    WORD  SMsgModeErrMsgRcvXEn;
}STRUCT_ZHHK1553_BC_SMSG_MODE;

typedef struct
{
    WORD  SMsgBranchEn;
    WORD  SMsgBranchToFrmId;
    WORD  SMsgBranchDataLocation;
    WORD  SMsgBranchDataMask;
    WORD  SMsgBranchDataExpectation;
    WORD  Reserve;
}STRUCT_ZHHK1553_BC_SMSG_BRANCH;

typedef struct
{
    DWORD SMsgChannel;
    DWORD SMsgFormat;
    DWORD SMsgRetryEn;
    DWORD SMsgGapTime;
    WORD SMsgBlock[36];
    STRUCT_ZHHK1553_BC_SMSG_MODE SMsgMode;
    STRUCT_ZHHK1553_BC_SMSG_BRANCH SMsgBranch;
}STRUCT_ZHHK1553_BC_SMSG;

typedef struct
{
    DWORD SFrmModeEndTrgOutEn;
    DWORD SFrmModeStartTrgOutEn;
    DWORD SFrmModeEndTrgIntEn;
    DWORD SFrmModeStartTrgIntEn;
    DWORD SFrmModeRxTriggerDivCnt;
}STRUCT_ZHHK1553_BC_SFRM_MODE;

typedef struct
{
    DWORD SFrmRun;
    DWORD SFrmPeriod;
    DWORD SFrmRepeatCnt;
    DWORD SFrmGapTime;
    DWORD SFrmSMsgCnt;
    STRUCT_ZHHK1553_BC_SFRM_MODE SFrmMode;
}STRUCT_ZHHK1553_BC_SFRMPAR;

typedef struct
{
    DWORD SFrmRun;
    DWORD SFrmPeriod;
    DWORD SFrmRepeatCnt;
    DWORD SFrmGapTime;
    DWORD SFrmSMsgCnt;
    STRUCT_ZHHK1553_BC_SFRM_MODE SFrmMode;
    STRUCT_ZHHK1553_BC_SMSG SFrmSMsg[2047];
}STRUCT_ZHHK1553_BC_SFRM;

typedef struct
{
    DWORD SFrmCnt;
    STRUCT_ZHHK1553_BC_SFRM SFrm[15];
}STRUCT_ZHHK1553_BC_SMFRM;

typedef struct
{
    DWORD RMsgBlockStatus;
    DWORD RMsgResposeTime0;
    DWORD RMsgResposeTime1;
    DWORD RMsgTimeTagLow;
    DWORD RMsgTimeTagHigh;
    WORD  RMsgBlock[36];
}STRUCT_ZHHK1553_RMSG;

typedef struct
{
    DWORD RetryEn;
    DWORD RetryNum;
    DWORD RetryIfMsgError;
    DWORD RetryIfStatusSet;
    DWORD RetryAlterChannelOnBus1;
    DWORD RetryAlterChannelOnBus2;
}STRUCT_ZHHK1553_BC_RETRY_CASE;

typedef struct
{
    DWORD MsgStopOnStatusSet;
    DWORD MsgStopOnMsgError;
    DWORD FrmStopOnStatusSet;
    DWORD FrmStopOnMsgError;
}STRUCT_ZHHK1553_BC_STOP_CASE;

typedef struct
{
    DWORD TerminalFlag;
    DWORD SubSystemFlag;
    DWORD ServiceRequest;
    DWORD Busy;
    DWORD DynamicBusControl;
}STRUCT_ZHHK1553_RT_STATUS_WORD;

typedef struct
{
    DWORD CmdTable[32][2][32];
}STRUCT_ZHHK1553_RT_ILLEGAL_CMD_TABLE;

typedef struct
{
    DWORD TxMode[32][32];
}STRUCT_ZHHK1553_RT_TX_MODE;

typedef struct
{
    DWORD Filter[32][2];
}STRUCT_ZHHK1553_MT_CMD_FILTER_TABLE;

typedef struct
{
    DWORD PulsePhysicalTypeSel;
    DWORD PulsePolaritySel;
    DWORD PulseWide;
}STRUCT_ZHHK1553_TRGTX_MODE;

typedef struct
{
    DWORD PulsePhysicalTypeSel;
    DWORD PulseEffectEdgeSel;
    DWORD PulseRawFxSel;
    DWORD PulseDivFxSel;
    DWORD PulseMulFxSel;
    DWORD PulseFxCnt;
    DWORD PulseMulFxClkCnt;
}STRUCT_ZHHK1553_TRGRX_MODE;


// Function Declaration
//************************************************************************
//********* COMMON OPERATION *********************************************
//************************************************************************
API int MF1553_Open (DevHandle *phZHHK1553, char *ServerAddr, DWORD ServerPort);
API int MF1553_Close (DevHandle hZHHK1553);
API int MF1553_Reset (DevHandle hZHHK1553);
API int MF1553_GetSerialNum (DevHandle hZHHK1553, DWORD *SerialNum0, DWORD *SerialNum1);

API int MF1553_ResetChannel (DevHandle hZHHK1553, DWORD ch);
API int MF1553_SpeedSel (DevHandle hZHHK1553, DWORD ch, DWORD Speed4MX1M);
API int MF1553_AddTimeTag (DevHandle hZHHK1553, DWORD ch, DWORD Enable);
API int MF1553_SetTimeTag (DevHandle hZHHK1553, DWORD ch, DWORD TimeTagLow, DWORD TimeTagHigh);
API int MF1553_GetTimeTag (DevHandle hZHHK1553, DWORD ch, DWORD *TimeTagLow, DWORD *TimeTagHigh);
API int MF1553_SetResponseTimeout (DevHandle hZHHK1553, DWORD ch, DWORD Timeout);

//************************************************************************
//********* BUS CONTROL MODE *********************************************
//************************************************************************
API int MF1553_BC_Init (DevHandle hZHHK1553, DWORD ch);
API int MF1553_BC_CfgWord (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_BC_RETRY_CASE *RetryCase, STRUCT_ZHHK1553_BC_STOP_CASE *StopCase);
API int MF1553_BC_WriteMsg (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, DWORD MsgId, STRUCT_ZHHK1553_BC_SMSG *Msg);
API int MF1553_BC_WriteMsgs (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, DWORD MsgId, DWORD MsgCnt, STRUCT_ZHHK1553_BC_SMSG *Msgs);
API int MF1553_BC_WriteFrmPar (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, STRUCT_ZHHK1553_BC_SFRMPAR *FrmPar);
API int MF1553_BC_WriteFrm (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, STRUCT_ZHHK1553_BC_SFRM *Frm);
API int MF1553_BC_UpdateFrm (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, STRUCT_ZHHK1553_BC_SFRM *Frm);
API int MF1553_BC_AddEndOfFrmList (DevHandle hZHHK1553, DWORD ch, DWORD FrmCnt);
API int MF1553_BC_GetCntOfFrmList (DevHandle hZHHK1553, DWORD ch, DWORD *FrmCnt);
API int MF1553_BC_FrmRunCtl (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, DWORD Run);
API int MF1553_BC_GetMsgInfo (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, DWORD MsgId, STRUCT_ZHHK1553_BC_SMSG *Msg);
API int MF1553_BC_GetMsgsInfo (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, DWORD MsgId, DWORD MsgCnt, STRUCT_ZHHK1553_BC_SMSG *Msgs);
API int MF1553_BC_GetFrmParInfo (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, STRUCT_ZHHK1553_BC_SFRMPAR *FrmPar);
API int MF1553_BC_GetFrmInfo (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, STRUCT_ZHHK1553_BC_SFRM *Frm);
API int MF1553_BC_WriteMsgDataBlock (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, DWORD MsgId, DWORD Offset, DWORD WordCnt, WORD *DataBuf);
API int MF1553_BC_Start (DevHandle hZHHK1553, DWORD ch);
API int MF1553_BC_Stop (DevHandle hZHHK1553, DWORD ch);
API int MF1553_BC_MsgBufferClear (DevHandle hZHHK1553, DWORD ch);
API int MF1553_BC_GetLastMsgId (DevHandle hZHHK1553, DWORD ch, DWORD *FrmId, DWORD *MsgId);
API int MF1553_BC_GetLastFrmId (DevHandle hZHHK1553, DWORD ch, DWORD *FrmId);
API int MF1553_BC_GetIntMsgId (DevHandle hZHHK1553, DWORD ch, DWORD *FrmId, DWORD *MsgId);
API int MF1553_BC_GetIntFrmId (DevHandle hZHHK1553, DWORD ch, DWORD *FrmId);
API int MF1553_BC_GetMsgCntTotal (DevHandle hZHHK1553, DWORD ch, DWORD *MsgCntTotal);
API int MF1553_BC_GetMsgNumNewly (DevHandle hZHHK1553, DWORD ch, DWORD *MsgNum);
API int MF1553_BC_ReadMsg (DevHandle hZHHK1553, DWORD ch, DWORD *FrmId, DWORD *MsgId, STRUCT_ZHHK1553_RMSG *Msg);
API int MF1553_BC_ReadMsgs (DevHandle hZHHK1553, DWORD ch, DWORD MsgNum, DWORD *FrmIds, DWORD *MsgIds, STRUCT_ZHHK1553_RMSG *Msgs);
API int MF1553_BC_ReadLastMsg (DevHandle hZHHK1553, DWORD ch, DWORD *FrmId, DWORD *MsgId, STRUCT_ZHHK1553_RMSG *Msg);
API int MF1553_BC_GetMsgRefresh (DevHandle hZHHK1553, DWORD ch, DWORD FrmId, DWORD MsgId, STRUCT_ZHHK1553_RMSG *Msg);
API int MF1553_BC_IsRunning (DevHandle hZHHK1553, DWORD ch);
API int MF1553_BC_TxTriggerMode (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_TRGTX_MODE *Mode);
API int MF1553_BC_TxTriggerCmd (DevHandle hZHHK1553, DWORD ch);
API int MF1553_BC_RxTriggerMode (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_TRGRX_MODE *Mode);

//************************************************************************
//********* RT MODE ******************************************************
//************************************************************************
API int MF1553_RT_Init (DevHandle hZHHK1553, DWORD ch);
API int MF1553_RT_RTAddr (DevHandle hZHHK1553, DWORD ch, DWORD AddrVector);
API int MF1553_RT_TxMode (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_RT_TX_MODE *TxMode);
API int MF1553_RT_SetTimeTagOnSync (DevHandle hZHHK1553, DWORD ch, DWORD ClearTimeTagOnSyncEn, DWORD LoadTimeTagOnSyncEn);
API int MF1553_RT_SetIllCmdTable (DevHandle hZHHK1553, DWORD ch, DWORD IllCmdTableEn, DWORD IllCmdRcvDataEn, STRUCT_ZHHK1553_RT_ILLEGAL_CMD_TABLE *IllCmdTable);
API int MF1553_RT_SetRTStatusWord (DevHandle hZHHK1553, DWORD ch, DWORD RTAddr, STRUCT_ZHHK1553_RT_STATUS_WORD *RTStatusWord);
API int MF1553_RT_VectorWordAutoClrEn (DevHandle hZHHK1553, DWORD ch, DWORD Enable);
API int MF1553_RT_SetVectorWord (DevHandle hZHHK1553, DWORD ch, DWORD RTAddr, DWORD VectorWord);
API int MF1553_RT_GetVectorWord (DevHandle hZHHK1553, DWORD ch, DWORD RTAddr, DWORD *VectorWord);
API int MF1553_RT_SetBitWord (DevHandle hZHHK1553, DWORD ch, DWORD RTAddr, DWORD BitWord);
API int MF1553_RT_WriteMsgDataBlock (DevHandle hZHHK1553, DWORD ch, DWORD RTAddr, DWORD SubAddr, DWORD WordCnt, WORD *DataBuf);
API int MF1553_RT_SendSaLoopBackCmd (DevHandle hZHHK1553, DWORD ch, DWORD RTAddr, DWORD SubAddr);
API int MF1553_RT_SetSaDepth (DevHandle hZHHK1553, DWORD ch, DWORD RTAddr, DWORD SubAddr, DWORD Depth);
API int MF1553_RT_MsgBufferClear (DevHandle hZHHK1553, DWORD ch);
API int MF1553_RT_GetMsgNumRxNewly (DevHandle hZHHK1553, DWORD ch, DWORD *MsgNum);
API int MF1553_RT_GetMsgNumTxNewly (DevHandle hZHHK1553, DWORD ch, DWORD *MsgNum);
API int MF1553_RT_ReadMsgRx (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_RMSG *Msg);
API int MF1553_RT_ReadMsgsRx (DevHandle hZHHK1553, DWORD ch, DWORD MsgNum, STRUCT_ZHHK1553_RMSG *Msgs);
API int MF1553_RT_ReadMsgTx (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_RMSG *Msg);
API int MF1553_RT_ReadMsgsTx (DevHandle hZHHK1553, DWORD ch, DWORD MsgNum, STRUCT_ZHHK1553_RMSG *Msgs);

//************************************************************************
//********* MT MODE ******************************************************
//************************************************************************
API int MF1553_MT_Init (DevHandle hZHHK1553, DWORD ch);
API int MF1553_MT_SetCmdFilterTable (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_MT_CMD_FILTER_TABLE *FilterTable);
API int MF1553_MT_Start (DevHandle hZHHK1553, DWORD ch);
API int MF1553_MT_Stop (DevHandle hZHHK1553, DWORD ch);
API int MF1553_MT_MsgBufferClear (DevHandle hZHHK1553, DWORD ch);
API int MF1553_MT_GetMsgNumNewly (DevHandle hZHHK1553, DWORD ch, DWORD *MsgNum);
API int MF1553_MT_ReadMsg (DevHandle hZHHK1553, DWORD ch, STRUCT_ZHHK1553_RMSG *Msg);
API int MF1553_MT_ReadMsgs (DevHandle hZHHK1553, DWORD ch, DWORD MsgNum, STRUCT_ZHHK1553_RMSG *Msgs);


#ifdef __cplusplus
}
#endif

#endif