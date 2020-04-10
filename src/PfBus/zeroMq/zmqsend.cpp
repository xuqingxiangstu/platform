#include "zmqsend.h"

namespace Pf
{
    namespace PfBus
    {
        zmqSend::zmqSend():
            mSocket(nullptr)
        {

        }

        zmqSend::~zmqSend()
        {

        }

        void zmqSend::init(const std::string &remoteIp, const std::string remotePort)
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

                mSocket = std::make_shared<zmq::socket_t>(mCtx, ZMQ_PUSH);

                mSocket->connect(addr);
            }
            catch(zmq::error_t err)
            {
                throw std::runtime_error(err.what());
            }
        }

        bool zmqSend::sendMsg(unsigned char *u8Msg, const unsigned int &u32MsgLen)
        {
            std::lock_guard<std::mutex> lk(m_Mutex);

            return mSocket->send(u8Msg, u32MsgLen);
        }

    }
}
