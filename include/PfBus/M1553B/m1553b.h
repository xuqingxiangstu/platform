#ifndef M1553B_H
#define M1553B_H

#include "m1553b_global.h"

//#define NO_1553B_CARD  1

#ifdef Q_OS_WIN
#include "./ZHHK1553/windows/defines.h"
#include "./ZHHK1553/windows/ZHHK1553_dll.h"
#endif

#ifdef Q_OS_LINUX
#ifndef NO_1553B_CARD
#include "./ZHHK1553/linux/defines.h"
#include "./ZHHK1553/linux/ZHHK1553_lib.h"
#endif
#endif

#include <mutex>
#include <memory>

namespace Pf
{
    namespace PfBus
    {
        class M1553BSHARED_EXPORT M1553B
        {        
            enum MsgFormat{
                BC_2_RT = 0,
                RT_2_RT
            };

            enum BusType{
                Bus_B,
                Bus_A
            };
        public:
            M1553B();
            ~M1553B();
        public:
            void init(std::string cardNum, std::string mode);

            bool receiveMsg(unsigned short &rt, unsigned short &sa, unsigned short *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut = 0xFFFFFFFF);

            bool Bc2RtMsg(unsigned short *u8Msg, unsigned int u32MsgLen, int rtAddr, int saAddr, int bus);

            bool Rt2RtMsg(int sRtAddr, int sSaAddr, int rRtAddr, int rSaAddr, int msgSize, int bus);
        private:
            void Rt2BcMsg(int rtAddr, int saAddr, int msgSize, int bus);
        private:
#ifndef NO_1553B_CARD
            DevHandle mDevHandle;
            std::string mServerAddr;
            DWORD mServerPort;
            int mChannel;
            STRUCT_ZHHK1553_RMSG mRcvMsg[512];
#endif
        };
    }
}

#endif // M1553B_H
