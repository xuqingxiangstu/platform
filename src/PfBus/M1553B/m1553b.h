#ifndef M1553B_H
#define M1553B_H

#include "m1553b_global.h"

#include "./ZHHK1553/include/ZHHK1553_lib.h"

#define NO_1553B_CARD  1

#include <mutex>
#include <memory>

namespace Pf
{
    namespace PfBus
    {
        class M1553BSHARED_EXPORT M1553B
        {

        public:
            M1553B();
            ~M1553B();
        public:
            void init(std::string cardNum, std::string mode);

            bool sendMsg(unsigned short *u8Msg, unsigned int u32MsgLen, int rtAddr, int saAddr);

            bool receiveMsg(unsigned short &rt, unsigned short &sa, unsigned short *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut = 0xFFFFFFFF);
        private:
            DevHandle mDevHandle;
            std::string mServerAddr;
            DWORD mServerPort;
            int mChannel;
        };
    }
}

#endif // M1553B_H
