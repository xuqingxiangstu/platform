#include "custommsg.h"

#include "zy.h"

#include "../virtualParams/virtualparams.h"

std::shared_ptr<customMsg> customMsg::mInstance = nullptr;

customMsg::customMsg()
    :mIcdObj(nullptr),
      mBusAdapter(nullptr),
      mUiAdapter(nullptr)
{

}

bool customMsg::exe(std::shared_ptr<frame> frameObj, const std::string &uuid, std::shared_ptr<Pf::PfIcdWorkBench::frameObj> icdObj, Pf::PfAdapter::Adapter *busObj, Pf::PfAdapter::Adapter *uiObj, const std::string &table, const std::string &coding)
{
    bool res = false;

    mRecordUuid = uuid;
    mIcdObj = icdObj;
    mBusAdapter = busObj;
    mUiAdapter = uiObj;

    if(XK_CUSTOM_MSG == table)
    {
        emit switchPermiss((qint64)static_cast<void *>(mBusAdapter), false);
        res = exe_Zy(coding);
        emit switchPermiss((qint64)static_cast<void *>(mBusAdapter), true);
    }
    else if(UI_CUSTOM_MSG == table) //界面内容消息触发
    {
        //mCurMsgType = CustomMsg;
        //更新参数
        virtualParams::getInstance()->setValue({"UI", UI_CUSTOM_MSG, coding}, mapValue());
    }
    else
    {

    }

    return res;
}

bool customMsg::exe_Zy(const std::string &coding)
{
    //执行
    zy obj(mRecordUuid, coding, mIcdObj, mBusAdapter, mUiAdapter);
}
