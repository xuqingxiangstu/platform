#include "analogout.h"

#include <sstream>
#include <cstring>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

namespace Pf
{
    namespace PfNi
    {
        AnalogOut::AnalogOut():
            mChannelSize(0),
            mSamplingRate(50),
            mSamplingSize(1),            
            isStarting(false),
            mTaskHandle(0),
            mOutType(VoltsOut),
            mMinValue(-10.0),
            mMaxValue(10.0)

        {

        }
        AnalogOut::~AnalogOut()
        {
            if (mTaskHandle != 0)
            {
                if(isStarting)
                    DAQmxStopTask(mTaskHandle);

                DAQmxClearTask(mTaskHandle);
                mTaskHandle = 0;
            }
        }

        void AnalogOut::initCard(const std::string &rName, const std::vector<int> &channelNums)
        {
            int32 NumWrite = 0;
            std::ostringstream strCode;
            int32       error = 0;
            int i = 0;
            char        errBuff[512] = { '\0' };

            DAQmxErrChk(DAQmxCreateTask("", &mTaskHandle));

            for(auto channel : channelNums)
            {
                strCode.str("");
                strCode << rName << "/ao" << channel;

                if(VoltsOut == mOutType)
                    DAQmxErrChk(DAQmxCreateAOVoltageChan(mTaskHandle, strCode.str().c_str(), "", mMinValue, mMaxValue, DAQmx_Val_Volts, NULL));
                else if(AmpsOut == mOutType)
                    DAQmxErrChk(DAQmxCreateAOCurrentChan(mTaskHandle, strCode.str().c_str(), "", mMinValue, mMaxValue, DAQmx_Val_Amps, NULL));


               // DAQmxErrChk(DAQmxSetChanAttribute(mTaskHandle, strCode.str().c_str(), DAQmx_AO_TermCfg, DAQmx_Val_RSE));
            }

            ///DAQmxErrChk(DAQmxCfgSampClkTiming(mTaskHandle, "", PXI_6722_SAMPLING_RATE, DAQmx_Val_Rising, DAQmx_Val_ContSamps, PXI_6722_SAMPLING_SIZE));
            ///< 测试面板出现同样问题
            DAQmxCfgSampClkTiming(mTaskHandle, "", mSamplingRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, mSamplingSize);

            ///DAQmxErrChk(DAQmxSetWriteAttribute(mTaskHandle, DAQmx_Write_RegenMode, DAQmx_Val_AllowRegen));

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

        void AnalogOut::setOutParam(const OutType &type, const double &minValue, const double &maxValue)
        {
            mOutType = type;
            mMinValue = minValue;
            mMaxValue = maxValue;
        }

        bool AnalogOut::writeValue(const std::vector<float64> &value)
        {
            bool Res = true;
            int32  NumWrite = 0;

            DAQmxWriteAnalogF64(mTaskHandle, mSamplingSize, 1, 0, DAQmx_Val_GroupByChannel, &value.at(0), &NumWrite, NULL);

            if (NumWrite != mChannelSize * mSamplingSize)
            {
                Res = false;
            }

            return Res;
        }

        bool AnalogOut::writeValue(const float64 *value)
        {
            bool Res = true;
            int32  NumWrite = 0;

            DAQmxWriteAnalogF64(mTaskHandle, mSamplingSize, 1, 0, DAQmx_Val_GroupByChannel, value, &NumWrite, NULL);

            if (NumWrite != mChannelSize * mSamplingSize)
            {
                Res = false;
            }

            return Res;
        }

        void AnalogOut::startTask()
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

        void AnalogOut::stopTask()
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

        int AnalogOut::getChannelSize()
        {
            return mChannelSize;
        }

        bool AnalogOut::setSamplingRateAndSize(const int &rate, const int &size)
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

        int AnalogOut::getSamplingSize()
        {
            return mSamplingSize;
        }

        int AnalogOut::getSamplingRate()
        {
            return mSamplingRate;
        }
    }

}
