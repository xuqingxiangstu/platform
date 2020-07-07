#ifndef _DEFINES_H_
#define _DEFINES_H_
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// BC¿ØÖÆ×ÖµÄÎ»
#define FMSG_BC_RT          0x00
#define FMSG_RT_RT          0x01
#define FMSG_BRDCST         0x02
#define FMSG_MC             0x04

// BC Bus Controller
#define BCMS_INVALID_WORD   0x0001
#define BCMS_ICRT_SYNCTYPE  0x0002
#define BCMS_ERR_WORD_LEN   0x0004
#define BCMS_ERR_RT_ADDR    0x0008
#define BCMS_GDBLOCK_TRANS  0x0010
#define BCMS_RETRY_COUNT0   0x0020
#define BCMS_RETRY_COUNT1   0x0040
#define BCMS_RETRY_COUNT3   0x0060
#define BCMS_ERR_CMD        0x0100
#define BCMS_NO_RP_TIMEOUT  0x0200
#define BCMS_FMT_ERR        0x0400
#define BCMS_STATUS_SET     0x0800
#define BCMS_ERR_FLAG       0x1000
#define BCMS_CH_BORA        0x2000
#define BCMS_SOM            0x4000
#define BCMS_EOM            0x8000

// RT Remote Terminal
#define RTMS_ERR_CMD        0x0001
#define RTMS_ERR_CMD2       0x0002
#define RTMS_ERR_ADDR       0x0004
#define RTMS_ERR_DATA       0x0008
#define RTMS_ICRT_SYNC      0x0010
#define RTMS_WCNT_ERR       0x0020
#define RTMS_CMD_ILL        0x0040
#define RTMS_ROB_OF         0x0080
#define RTMS_NO_RP_TIMEOUT  0x0200
#define RTMS_FMT_ERR        0x0400
#define RTMS_RT_RT_FMT      0x0800
#define RTMS_ERR_FLAG       0x1000
#define RTMS_CH_BORA        0x2000
#define RTMS_SOM            0x4000
#define RTMS_EOM            0x8000

// MT Monitor
#define MTMS_ERR_CMD        0x0001
#define MTMS_ERR_CMD2       0x0002
#define MTMS_ERR_ADDR       0x0004
#define MTMS_ERR_DATA       0x0008
#define MTMS_ICRT_SYNC      0x0010
#define MTMS_WCNT_ERR       0x0020
#define MTMS_ROB_OF         0x0040
#define MTMS_GOOD           0x0080
#define MTMS_NO_RP_TIMEOUT  0x0200
#define MTMS_FMT_ERR        0x0400
#define MTMS_RT_RT_FMT      0x0800
#define MTMS_ERR_FLAG       0x1000
#define MTMS_CH_BORA        0x2000
#define MTMS_SOM            0x4000
#define MTMS_EOM            0x8000

#ifdef __cplusplus
}
#endif

#endif