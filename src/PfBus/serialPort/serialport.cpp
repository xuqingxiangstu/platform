#include "serialport.h"
#include <sstream>
#include <stdexcept>
namespace Pf{
    namespace PfBus
    {
        void SerialPort::Init(std::string name,unsigned long u32BandRate,unsigned char u8Bytesize,unsigned char u8StopBit,
                  unsigned char u8Parity, unsigned long u32FBinary, unsigned long u32FParity)
        {
            COMMTIMEOUTS temp;
            std::ostringstream buf;
            char cStr[20] ={0};
            //获取串口位置及相应参数
            m_strPortName= name;
            char cSub[10] = "\\\\.\\";
            sprintf(cStr, "%s%s", cSub,name.c_str());
            m_comStruct.u32BandRate = u32BandRate;//波特率
            m_comStruct.u8ByteSize  = u8Bytesize; //数据位  4-8
            m_comStruct.u8StopBits  = u8StopBit;  //停止位  0，1，2 = 1bit, 1.5bit, 2bit
            m_comStruct.u8Parity    = u8Parity;   //奇偶校验位 0-4=None,Odd,Even,Mark,Space
            m_comStruct.u32FBinary  = u32FBinary; //二进制使能位 默认为1
            m_comStruct.u32FParity  = u32FParity; //校验使能位 默认为1
            TCHAR cDevName[100] = { 0 };
            memset(cDevName, 0, sizeof(TCHAR) * 100);
            #ifdef UNICODE
            MultiByteToWideChar(CP_ACP, 0, cStr, -1, cDevName, 20);
            #else
            strcpy_s((char*)cDevName, 20, cStr);
            #endif
            //打开串口
           Open(cDevName);
           // 设置缓冲区大小
           // SetupComm(m_hSerialPort, 1024, 1024);	//设置输入输出缓冲区大小，默认缓冲区大小
           // 设置端口DCB
           SetDCB();
           memset(&temp, 0, sizeof(COMMTIMEOUTS));
           GetCommTimeouts(m_hSerialPort, &temp);
           temp.ReadIntervalTimeout = 0;          /* Maximum time between read chars. */
           temp.ReadTotalTimeoutMultiplier = 0;   /* Multiplier of characters.        */
           temp.ReadTotalTimeoutConstant = 1;     /* Constant in milliseconds.        */
           temp.WriteTotalTimeoutMultiplier = 0;  /* Multiplier of characters.        */
           temp.WriteTotalTimeoutConstant = 0;    /* Constant in milliseconds.        */
           SetCommTimeouts(m_hSerialPort, &temp);
        }

        void SerialPort::Open(TCHAR * tcComName)
        {
            std::ostringstream buf;
            //打开串口
            m_hSerialPort = CreateFile(tcComName,    // communication port string (COMX)
                GENERIC_READ | GENERIC_WRITE,	     // read/write types
                0,								    // comm devices must be opened with exclusive access
                NULL,                               // no security attributes
                OPEN_EXISTING,                      // comm devices must use OPEN_EXISTING
                FILE_ATTRIBUTE_NORMAL, // Async I/O (重叠读写)
                                       //FILE_FLAG_OVERLAPPED,
                NULL);
            if (m_hSerialPort == INVALID_HANDLE_VALUE)
            {
                buf << m_strPortName << " Open failed!";
                throw std::runtime_error(buf.str());
            }
        }

        void SerialPort::SetDCB()
        {
           unsigned long u32Error =0;
           DCB dcb;
           std::ostringstream buf;
           memset(&dcb,0,sizeof(DCB)); //Clear DCB
           if(!GetCommState(m_hSerialPort,&dcb))
           {
               u32Error = COM_GETPARAM; // 获取端口参数失败
           }
           dcb.BaudRate = m_comStruct.u32BandRate;
           dcb.ByteSize = m_comStruct.u8ByteSize;
           dcb.StopBits = m_comStruct.u8StopBits;
           dcb.Parity   = m_comStruct.u8Parity;
           dcb.fBinary  = m_comStruct.u32FBinary;
           dcb.fParity  = m_comStruct.u32FParity;
           if((!u32Error)&&(!SetCommState(m_hSerialPort,&dcb)))
           {
                  buf << "普通串口 " << m_strPortName << " 设置端口参数失败";
                  throw std::runtime_error(buf.str());
            }
        }

        bool SerialPort::SndUartData(unsigned char *u8SndBuf, DWORD dw16DataLength)
        {
            DWORD ulBytes = 0;
            WriteFile(m_hSerialPort, u8SndBuf, dw16DataLength, &ulBytes, &m_ioWrite);
            FlushFileBuffers(m_hSerialPort);
            if (ulBytes == dw16DataLength)
            {
        #ifdef MSG_LOG
                //写通信数据文件
                m_BinaryRW.Write((char *)u8SndBuf, ulBytes);
        #endif
                return 1;//发送成功
            }
            else
                return 0;//发送失败
        }

        bool SerialPort::RcvUartData(unsigned char *u8RcvBuf, DWORD dw16DataLength, DWORD * u16Bytes, unsigned int u32TimeOut)
        {
            bool res = false;
            DWORD dwBytesToRead = dw16DataLength;
            unsigned long ulError = 0;
            COMSTAT ComStat;
            COMMTIMEOUTS temp;
            memset(&temp, 0, sizeof(COMMTIMEOUTS));
            GetCommTimeouts(m_hSerialPort, &temp);
            temp.ReadIntervalTimeout = 0;          /* Maximum time between read chars. */
            temp.ReadTotalTimeoutMultiplier = 0;   /* Multiplier of characters.        */
            temp.ReadTotalTimeoutConstant = u32TimeOut;     /* Constant in milliseconds.        */
            temp.WriteTotalTimeoutMultiplier = 0;  /* Multiplier of characters.        */
            temp.WriteTotalTimeoutConstant = 0;    /* Constant in milliseconds.        */
            SetCommTimeouts(m_hSerialPort, &temp);

            ClearCommError(m_hSerialPort, &ulError, &ComStat);
            if(ComStat.cbInQue>0)
            {
              ReadFile(m_hSerialPort, u8RcvBuf, dwBytesToRead, u16Bytes, &m_ioRead);
              //FlushFileBuffers(m_hSerialPort);
              res = true;
            }

            if (ulError & CE_RXPARITY)
            {
                //buf << "串口奇偶校验错误" << endl;
                //throw runtime_error(buf.str());
               res =0;
            }
            FlushFileBuffers(m_hSerialPort);
            if(res)
            {

            }
            return res;
        }

        SerialPort::SerialPort()
        {
            memset(&m_ioRead, 0, sizeof(OVERLAPPED));
            memset(&m_ioWrite, 0, sizeof(OVERLAPPED));

        }

        SerialPort::~SerialPort()
        {
            if(m_hSerialPort != INVALID_HANDLE_VALUE)
            {
                CloseHandle(m_hSerialPort);
            }
        }
    }
}
