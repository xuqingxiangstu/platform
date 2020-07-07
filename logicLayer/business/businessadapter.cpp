#include "businessadapter.h"

#include "./xk/xkbusiness.h"
#include "./dm/dmbusiness.h"
#include "./czxk/czxkbusiness.h"
#include "./middle/middlebusiness.h"
#include "./be/bebusiness.h"
#include "./fe/febusiness.h"
#include "./c3/c3business.h"
#include "./s93/s93business.h"
#include "./m1553b/m1553bbusiness.h"
#include "./jg/jgbusiness.h"

std::shared_ptr<businessAdapter> businessAdapter::mInstance = nullptr;

businessAdapter::businessAdapter()
{
    //初始化各业务逻辑
    mBusinessManagement[FRAME_BE] = std::make_shared<beBusiness>();
    mBusinessManagement[FRAME_FE] = std::make_shared<feBusiness>();
    mBusinessManagement[FRAME_MIDDLE] = std::make_shared<middleBusiness>();
    mBusinessManagement[FRAME_M1553B] = std::make_shared<m1553bBusiness>();
    mBusinessManagement[FRAME_XK] = std::make_shared<xkBusiness>();
    mBusinessManagement[FRAME_DM] = std::make_shared<dmBusiness>();
    mBusinessManagement[FRAME_CZXK] = std::make_shared<czxkBusiness>();
    mBusinessManagement[FRAME_C3] = std::make_shared<c3Business>();
    mBusinessManagement[FRAME_93] = std::make_shared<s93Business>();
    mBusinessManagement[FRAME_JG] = std::make_shared<jgBusiness>();
}


std::shared_ptr<business> businessAdapter::getBusiness(const std::string &key)
{
    bool res = false;

    auto itor = mBusinessManagement.find(key);
    if(itor == mBusinessManagement.end())
    {
        return nullptr;
    }

    return (itor->second);
}
