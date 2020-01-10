#include "UnicastUdp.h"

#include <sstream>

namespace Pf
{
    namespace PfBus
    {

        UnicastUdp::UnicastUdp(void)
        {

        }

        UnicastUdp::~UnicastUdp(void)
        {
            if (mSock != NULL)
            {
                shutdown(mSock, SD_BOTH);
                closesocket(mSock);
            }
        }

        void UnicastUdp::unInit()
        {
            delete this;
        }

        void UnicastUdp::init(std::string strLocalIp, std::string strLocalPort, std::string strRemoteIp, std::string strRemotePort)
        {
            int i32LocalPort = 0;
            int i32RemotePort = 0;

            WORD		wVersionRequested;
            WSADATA		wsaData;
            std::ostringstream strErr;

            wVersionRequested = MAKEWORD(2, 0);

            sscanf(strLocalPort.c_str(), "%d", &i32LocalPort);
            sscanf(strRemotePort.c_str(), "%d", &i32RemotePort);

            if (WSAStartup(wVersionRequested, &wsaData) != 0)
            {
                throw std::runtime_error("[ERROR] UnicastUdp WSAStartup error\n");
            }

            mSock = socket(AF_INET, SOCK_DGRAM, 0);

            if (INVALID_SOCKET != mSock)
            {
                struct addrinfo *lpHost = NULL;
                struct addrinfo *ptr = NULL;
                struct addrinfo hints;

                ZeroMemory(&hints, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_IP;
                getaddrinfo((LPCSTR)strLocalIp.c_str(), NULL, &hints, &lpHost);

                memset(&mrAddr, 0, sizeof(mrAddr));
                mrAddr.sin_family = AF_INET;

                if (i32LocalPort == 0)
                {
                    i32LocalPort = i32RemotePort;
                }

                mrAddr.sin_port = htons(i32LocalPort);
                struct sockaddr_in *Addr = (struct sockaddr_in *)lpHost->ai_addr;

                if (strLocalIp == "")
                {
                    mrAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
                }
                else
                {
                    mrAddr.sin_addr.s_addr = Addr->sin_addr.s_addr;
                }


                ZeroMemory(&hints, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_IP;

                getaddrinfo((LPCSTR)strRemoteIp.c_str(), NULL, &hints, &lpHost);

                memset(&msAddr, 0, sizeof(msAddr));
                msAddr.sin_family = AF_INET;
                msAddr.sin_port = htons(i32RemotePort);
                struct sockaddr_in *Addr1 = (struct sockaddr_in *)lpHost->ai_addr;
                msAddr.sin_addr.s_addr = Addr1->sin_addr.s_addr;

                if (bind(mSock, (struct sockaddr*)(&mrAddr), sizeof(struct sockaddr_in)) != 0)
                {
                    strErr.str("");
                    strErr << "[ERROR]UnicastUdp 绑定Ip失败，localip = (" << strLocalIp << ") localport = (" << strLocalPort << ")"		\
                        << " remoteip = (" << strRemoteIp << ") remoteport = (" << strRemotePort << ")";
                    throw std::runtime_error(strErr.str());
                }
            }
            else
            {
                strErr.str("");
                strErr << "[ERROR]UnicastUdp 创建socket失败，localip = (" << strLocalIp << ") localport = (" << strLocalPort << ")"		\
                    << " remoteip = (" << strRemoteIp << ") remoteport = (" << strRemotePort << ")";
                throw std::runtime_error(strErr.str());
            }
        }

        bool UnicastUdp::sendMsg(unsigned char *u8Msg, unsigned int u32MsgLen)
        {
            bool Res = false;
            int iReturnValue = sendto(mSock, (char*)u8Msg, u32MsgLen, 0, (sockaddr*)(&msAddr), sizeof(msAddr));
            if (SOCKET_ERROR == iReturnValue)
            {
                Res = false;
            }
            else
            {
                Res = true;
            }
            return Res;
        }

        bool UnicastUdp::receiveMsg(unsigned char *u8Msg, unsigned int *u32MsgLen, const unsigned int u32RcvMax, const unsigned int u32TimeOut)
        {
            bool bRes = false;
            int count = sizeof(struct sockaddr_in);
            int RecvLen = 0, ret;
            struct fd_set fds;
            char *rBuf = new char[u32RcvMax + 1];
            memset(rBuf, 0, (u32RcvMax + 1) * sizeof(char));

            TIMEVAL stTime;
            TIMEVAL *pstTime = NULL;

            if (INFINITE != u32TimeOut)
            {
                stTime.tv_sec = u32TimeOut / 1000;
                stTime.tv_usec = (u32TimeOut % 1000) * 1000;
                pstTime = &stTime;
            }

            FD_ZERO(&fds);
            FD_SET(mSock, &fds);

            ret = select(NULL, &fds, NULL, NULL, pstTime);
            switch (ret)
            {
            case SOCKET_ERROR:						// select出错
                break;
            case 0:							// 超时
                break;
            default:
                if (FD_ISSET(mSock, &fds))
                {
                    RecvLen = recvfrom(mSock, rBuf, u32RcvMax, 0, (struct sockaddr*)(&mrAddr), &count);
                    if (RecvLen > 0)
                    {
                        memcpy_s((char*)u8Msg, u32RcvMax, rBuf, RecvLen);
                        *u32MsgLen = RecvLen;
                        bRes = true;
                    }
                    else
                    {
                        bRes = false;
                    }
                }
                break;
            }

            delete rBuf;
            rBuf = NULL;

            return bRes;
        }
    }
}
