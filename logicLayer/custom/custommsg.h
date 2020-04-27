#ifndef CUSTOMMSG_H
#define CUSTOMMSG_H

#include <memory>
#include <QObject>

#include "../../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"
#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

#include "../flow/frame.h"

#include "zy.h"

class customMsg : public QObject
{
    Q_OBJECT
public:
    static std::shared_ptr<customMsg> getInstance()
    {
        if(mInstance.get() == nullptr)
        {
            mInstance = std::make_shared<customMsg>();
        }
        return mInstance;
    }

    customMsg();
signals:
    void switchPermiss(qint64, bool);
public:
    bool exe(std::shared_ptr<frame> frameObj, const std::string &uuid, std::shared_ptr<Pf::PfIcdWorkBench::frameObj> icdObj, Pf::PfAdapter::Adapter *busObj, Pf::PfAdapter::Adapter *uiObj, const std::string &table, const std::string &coding);
private:
    bool exe_Zy(const std::string &coding);
private:
    static std::shared_ptr<customMsg> mInstance;
    Pf::PfAdapter::Adapter *mBusAdapter;
    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> mIcdObj;
    Pf::PfAdapter::Adapter *mUiAdapter;
    std::string mRecordUuid;
};

#endif // CUSTOMMSG_H
