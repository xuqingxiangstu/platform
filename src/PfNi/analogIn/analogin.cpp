#include "analogin.h"

#include <sstream>
#include <cstring>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

namespace Pf
{
    namespace PfNi
    {
        AnalogIn::AnalogIn():
        mChannelSize(0),
        mSamplingRate(50),
        mSamplingSize(1),
        isStarting(false),
        mTaskHandle(0)
        {

        }

        AnalogIn::~AnalogIn()
        {
            if (mTaskHandle != 0)
            {
                if(isStarting)
                    DAQmxStopTask(mTaskHandle);

                DAQmxClearTask(mTaskHandle);
                mTaskHandle = 0;
            }
        }

        void AnalogIn::initCard(const std::string &rName, const std::vector<int> &channelNums)
        {
            std::ostringstream strCode;
            int32       error = 0;
            int i = 0;
            char        errBuff[512] = { '\0' };

            DAQmxErrChk(DAQmxCreateTask("", &mTaskHandle));

            for(auto channel : channelNums)
            {
                strCode.str("");
                strCode << rName << "/ai" << channel;

                DAQmxErrChk(DAQmxCreateAIVoltageChan(mTaskHandle, strCode.str().c_str(), "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));

                //设置输入模式
                DAQmxErrChk(DAQmxSetAITermCfg(mTaskHandle, strCode.str().c_str(), DAQmx_Val_RSE));
            }

            //DAQmx_Val_ContSamps 连续采样,直至停止任务
            DAQmxErrChk(DAQmxCfgSampClkTiming(mTaskHandle, "", mSamplingRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, mSamplingSize));

            //DAQmxSetWriteAttribute(mTaskHandle, DAQmx_Write_RegenMode, DAQmx_Val_AllowRegen);

            DAQmxErrChk(DAQmxGetTaskAttribute(mTaskHandle, DAQmx_Task_NumChans, &mChannelSize));            
        Error:
            if( DAQmxFailed(error) )
            {

                DAQmxGetExtendedErrorInfo(errBuff, 512);

                if (mTaskHandle != 0)
                {
                    DAQmxClearTask(mTaskHandle);
                    mTaskHandle = 0;
                }

                std::string errCode;
                errCode += rName;
                errCode += "  init error(";
                errCode += std::string(errBuff);
                errCode + ")\n ";
                throw std::runtime_error(errCode);
            }            
        }

        bool AnalogIn::readValue(float64 *rValue, int &rSize)
        {
            bool res = false;
            int32     numRead = 0;           

            if(DAQmxSuccess == DAQmxReadAnalogF64(mTaskHandle, mSamplingSize, 10.0, DAQmx_Val_GroupByScanNumber, rValue, mSamplingSize * mChannelSize, &numRead, NULL))
            {
                rSize = mChannelSize * numRead;

                res = true;
            }

            return res;
        }

        void AnalogIn::startTask()
        {
            if(!isStarting)
            {
                if(mTaskHandle != 0)
                {
                    DAQmxStartTask(mTaskHandle);
                    isStarting = true;
                }
            }
        }

        void AnalogIn::stopTask()
        {
            if(isStarting)
            {
                if(mTaskHandle != 0)
                {
                    DAQmxStopTask(mTaskHandle);
                    isStarting = false;
                }
            }
        }

        int AnalogIn::getChannelSize()
        {
            return mChannelSize;
        }

        int AnalogIn::getSamplingSize()
        {
            return mSamplingSize;
        }

        bool AnalogIn::setSamplingRateAndSize(const int &rate, const int &size)
        {
            bool res = false;

            if(DAQmxSuccess == DAQmxCfgSampClkTiming(mTaskHandle, "", rate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, size))
            {
                mSamplingRate = rate;
                mSamplingSize = size;

                res = true;
            }

            return res;
        }

        int AnalogIn::getSamplingRate()
        {
            return mSamplingRate;
        }

        int AnalogIn::registerInEvent(DAQmxEveryNSamplesEventCallbackPtr ptr, void *data)
        {
            if(isStarting)
                return 1;

            if(mTaskHandle == 0)
                return 2;

            DAQmxRegisterEveryNSamplesEvent(mTaskHandle,DAQmx_Val_Acquired_Into_Buffer,mSamplingSize,0,ptr,data);

            return 0;
        }
    }
}
