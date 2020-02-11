#include "relayctrl.h"

#include "../niSwitch/niswitch.h"

#include <chrono>
#include <thread>

namespace Pf
{
    namespace PfNi
    {

        RelayCtrl::RelayCtrl():
            mChannelSize(10)
        {
        }

        RelayCtrl::~RelayCtrl()
        {
            if(mSession != 0)
            {
                //断开所有继电器
                niSwitch_DisconnectAll(mSession);

                niSwitch_close(mSession);
            }
        }

        int RelayCtrl::getChannelSize()
        {
            return mChannelSize;
        }


        void RelayCtrl::initCard(const std::string &rName, const int &channelSize)
        {
            if(VI_SUCCESS != niSwitch_init(ViRsrc(rName.c_str()),  VI_TRUE, VI_TRUE, &mSession))
            {
                throw std::runtime_error("OpenSwitchCard fail");
            }

            ///断开所有继电器
            niSwitch_DisconnectAll(mSession);

            setChannelSize(channelSize);
        }


        bool RelayCtrl::openRelay(const int &channel)
        {
            char relayName[1024] = {0};

            if(mSession == 0)
                return false;

            if(channel > mChannelSize)
                return false;

            sprintf(relayName,"K%d",channel);
            ViInt32 Vistatus = 0;

            niSwitch_GetRelayPosition(mSession, relayName, &Vistatus);

            if(Vistatus == NISWITCH_VAL_OPEN)
            {
                niSwitch_RelayControl(mSession, relayName, NISWITCH_VAL_CLOSE_RELAY);
            }
            //防止开关过快烧毁设备 2.开关动作需要延时一段时间后采集板卡才能收到信号
            std::this_thread::sleep_for(std::chrono::nanoseconds(25000));

            return true;
        }


        bool RelayCtrl::closeRelay(const int &channel)
        {
            char relayName[1024] = {0};

            if(mSession == 0)
                return false;

            if(channel > mChannelSize)
                return false;

            ViInt32 Vistatus = 0;
            sprintf(relayName,"K%d",channel);

            niSwitch_GetRelayPosition(mSession, relayName, &Vistatus);

            if(Vistatus == NISWITCH_VAL_CLOSED)
            {
                niSwitch_RelayControl(mSession, relayName, NISWITCH_VAL_OPEN_RELAY);
            }
            //1.防止开关过快烧毁设备 2.开关动作需要延时一段时间后采集板卡才能收到信号
            std::this_thread::sleep_for(std::chrono::nanoseconds(25000));

            return true;
        }


        void RelayCtrl::setChannelSize(const int &size)
        {
            mChannelSize = size;
        }
    }
}
