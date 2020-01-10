#include "digitalio.h"

#include <iterator>
#include <sstream>
#include <iostream>
#include <cstring>
#include <stdexcept>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

namespace Pf
{
    namespace PfNi
    {
        DigitalIn::DigitalIn():
            mPortSize(0),
            mDiData(nullptr),
            mTaskHandle(0)
        {
        }

        DigitalIn::~DigitalIn()
        {
            if (mTaskHandle != 0)
            {
                DAQmxStopTask(mTaskHandle);
                DAQmxClearTask(mTaskHandle);
            }
        }

        /**
         * @brief initCard  初始化板卡
         * @param[in] rName 板卡虚拟名称（NiMax中查看）
         * @param[in] ports 输入端口号，NiMax中查看
         * @exception   open fail
         */
        void DigitalIn::initCard(const std::string &rName, const std::vector<int> &ports)
        {
            std::ostringstream strCode;
            int32 error = 0;
            char errBuff[512] = { '\0' };

            DAQmxErrChk(DAQmxCreateTask("", &mTaskHandle));

            for(int portIndex : ports)
            {
                strCode.str("");
                strCode << rName << "/port" << portIndex;
                DAQmxErrChk(DAQmxCreateDIChan(mTaskHandle, strCode.str().c_str(), "", DAQmx_Val_ChanForAllLines));
            }

            DAQmxGetTaskAttribute(mTaskHandle, DAQmx_Task_NumChans, &mPortSize);

            mDiData = std::make_shared<unsigned char>(mPortSize);

            memset(mDiData.get(), 0, mPortSize*sizeof(unsigned char));

            DAQmxStartTask(mTaskHandle);
        Error:

            if (DAQmxFailed(error))
            {
                DAQmxGetExtendedErrorInfo(errBuff, 512);

                if (mTaskHandle != 0)
                {
                    DAQmxStopTask(mTaskHandle);
                    DAQmxClearTask(mTaskHandle);
                }

                mPortSize = 0;
                std::string errCode;
                errCode += rName;
                errCode += "  init error \n ";
                throw std::runtime_error(errCode);
            }
        }

        /**
         * @brief readValue 读取输入值
         * @param[out] value    与初始化端口一一对应（一个端口为8个输入信号）
         * @return 读取状态
         * - true：成功
         * - false：失败
         */
        bool DigitalIn::readValue(std::vector<unsigned char> &value)
        {
            bool res = false;

            if(DAQmxSuccess == DAQmxReadDigitalU8(mTaskHandle, 1, 10.0, DAQmx_Val_GroupByChannel, mDiData.get(), 1 * mPortSize, NULL, NULL))
            {
                res = true;
                for(int index = 0; index < mPortSize; index++)
                {
                    value.push_back(mDiData.get()[index]);
                }
            }

            return res;
        }

        DigitalOut::DigitalOut():
            mTaskHandle(0),
            mPortSize(0),
            mDoData(nullptr)
        {

        }

        DigitalOut::~DigitalOut()
        {
            if (mTaskHandle != 0)
            {
                DAQmxStopTask(mTaskHandle);
                DAQmxClearTask(mTaskHandle);
            }
        }

        /**
         * @brief initCard  初始化板卡
         * @param[in] rName 板卡虚拟名称（NiMax中查看）
         * @param[in] ports 输入端口号，NiMax中查看
         * @exception   open fail
         */
        void DigitalOut::initCard(const std::string &rName, const std::vector<int> &ports)
        {
            std::ostringstream strCode;
            int32 written = 0;
            int32 error = 0;
            char errBuff[512] = { '\0' };


            DAQmxErrChk(DAQmxCreateTask("", &mTaskHandle));

            for(int portIndex : ports)
            {
                strCode.str("");
                strCode << rName << "/port" << portIndex;
                DAQmxErrChk(DAQmxCreateDOChan(mTaskHandle, strCode.str().c_str(), "", DAQmx_Val_ChanForAllLines));
            }

            DAQmxGetTaskAttribute(mTaskHandle, DAQmx_Task_NumChans, &mPortSize);

            mDoData = std::make_shared<unsigned char>(mPortSize);
            for(int index = 0; index < mPortSize; index++)
            {
                mDoData.get()[index] = 0xFF;    //1为关
            }

            DAQmxWriteDigitalU8(mTaskHandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, mDoData.get(), &written, NULL);

            DAQmxStartTask(mTaskHandle);
        Error:

            if (DAQmxFailed(error))
            {
                DAQmxGetExtendedErrorInfo(errBuff, 512);

                if (mTaskHandle != 0)
                {
                    DAQmxStopTask(mTaskHandle);
                    DAQmxClearTask(mTaskHandle);
                }

                mPortSize = 0;

                throw std::runtime_error(errBuff);
            }
        }

        /**
         * @brief setValue  设置输出状态
         * @param[in] portIndex 第几个port(每个port有8路)
         * @param[in] channel 某个port的第几通道数
         * @param[in] state 状态
         * - true： 高状态
         * - false：低状态
         * @return 是否成功
         * - true：成功
         * - false：失败
         */
        int DigitalOut::setValue(const int &portIndex, const unsigned char &channel, const bool &state)
        {
            int32 written = 0;

            //port检查
            if(portIndex > (mPortSize - 1))
            {
                return 0;
            }

            //通道检查
            if((channel + 1) > 8)
            {
                return 1;
            }

            DAQmxReadDigitalU8(mTaskHandle, 1, 10.0, DAQmx_Val_GroupByChannel, mDoData.get(), 1 * mPortSize, NULL, NULL);
            //写1为断开，写0为开
            if (!state)
            {
                mDoData.get()[portIndex] |= (1 << channel);
            }
            else
            {
                mDoData.get()[portIndex] &= (~(1 << channel));
            }

            DAQmxWriteDigitalU8(mTaskHandle, 1, 1, 10.0, DAQmx_Val_GroupByChannel, mDoData.get(), &written, NULL);

            return -1;
        }
    }
}
