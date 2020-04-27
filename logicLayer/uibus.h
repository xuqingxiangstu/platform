#ifndef UIBUS_H
#define UIBUS_H

#include <memory>

#include "../src/PfAdapter/PfAdapterManager/pfadaptermanager.h"


class uiBus
{
public:
    uiBus();
    static std::shared_ptr<uiBus> getInstance()
    {
        if(mInstance.get() == nullptr)
        {
            mInstance = std::make_shared<uiBus>();
        }
        return mInstance;
    }

    void setUiAdapter(Pf::PfAdapter::Adapter *obj){mAdapter = obj;}
    Pf::PfAdapter::Adapter *getUiAdapter(){return mAdapter;}
private:
    static std::shared_ptr<uiBus> mInstance;
    Pf::PfAdapter::Adapter *mAdapter;
};

#endif // UIBUS_H
