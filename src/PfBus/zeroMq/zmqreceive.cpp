#include "zmqreceive.h"

namespace Pf
{
    namespace PfBus
    {
        zmqReceive::zmqReceive():
            mSocket(nullptr)
        {

        }

        zmqReceive::~zmqReceive()
        {
        }

        void zmqReceive::init(const std::string &remoteIp, const std::string &remotePort)
        {
            try
            {
                mIpAddr = remoteIp;
                mPort = remotePort;

                std::string addr;

                addr += "tcp://";
                if(mIpAddr == "")
                    addr += "*";
                addr += mIpAddr;
                addr += ":";
                addr += mPort;

                mSocket = std::make_shared<zmq::socket_t>(mCtx, ZMQ_PULL);

                mSocket->bind(addr);

                ///清空之前消息
                clearMsg();
            }
            catch(zmq::error_t err)
            {
                throw std::runtime_error(err.what());
            }
        }

        void zmqReceive::clearMsg()
        {
            return ;
            int timeout = 500;//ms

            mSocket->setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));

            unsigned char u8Msg[1024] = {0};
            unsigned int size = 0;
            while(1)
            {
                size = mSocket->recv(u8Msg, 1024);
                if(size == 0)
                    break;
            }

            timeout = -1;
            mSocket->setsockopt(ZMQ_RCVTIMEO, &timeout);
        }

        bool zmqReceive::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvSize, const unsigned int u32TimeOut)
        {
            bool res = false;

            std::lock_guard<std::mutex> lk(m_Mutex);
            *u32MsgLen = mSocket->recv(u8Msg, u32RcvSize);

            if(*u32MsgLen > 0)
                res = true;

            return res;
        }
    }
}


