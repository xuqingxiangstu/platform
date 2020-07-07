#include "frame.h"

#include "../../src/PfSql/paramsTable/paramstable.h"
#include "../../src/PfSmlModel/randaModel/randamodel.h"
#include "../../src/PfSmlModel/temperatureModel/temperatureModel.h"
#include "../../src/PfIcdWorkBench/icdData/datatype.h"

#include "../../src/PfCommon/tools/ut_error.h"

#include <QString>
#include <QDebug>

/***********************************************/

void head1553B::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;
    const char *point = nullptr;

    tmpEle = xmlEle->FirstChildElement("model");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["model"] = std::string(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("bus");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["bus"] = std::string(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("RT");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["RT"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("SA");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["SA"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("S_RT");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["S_RT"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("S_SA");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["S_SA"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("R_RT");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["R_RT"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("R_SA");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["R_SA"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("DATA_SIZE");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["DATA_SIZE"] = std::atoi(point);
        }
    }
}

Json::Value head1553B::serialize()
{
    return mJsonV;
}

/***********************************************/

void headBe::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;
    const char *point = nullptr;

    tmpEle = xmlEle->FirstChildElement("head_frame_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_frame_type"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_sys_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_sys_type"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_sys_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_sys_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_node_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_node_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_sys_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_sys_type"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_sys_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_sys_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_node_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_node_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_d_num");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_d_num"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_data");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_data"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dev_num");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dev_num"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_modle_num");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_modle_num"] = std::atoi(point);
        }
    }
}

Json::Value headBe::serialize()
{
    return mJsonV;
}

/***********************************************/

void headMiddle::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;
    const char *point = nullptr;

    tmpEle = xmlEle->FirstChildElement("head_frame_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_frame_type"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_ask_flag");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_ask_flag"] = point;
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_sys_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_sys_type"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_sys_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_sys_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_node_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_node_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_user");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_user"] = point;
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_src_version");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_version"] = point;
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_sys_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_sys_type"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_sys_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_sys_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_node_code");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_node_code"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_user");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_user"] = point;
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_version");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_version"] = point;
        }
    }    

    tmpEle = xmlEle->FirstChildElement("head_info_word_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_info_word_type"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_d_num");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_d_num"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_data");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_data"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dev_num");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dev_num"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_modle_num");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_modle_num"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_reserve");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_reserve"] = point;
        }
    }
}

Json::Value headMiddle::serialize()
{    
    return mJsonV;
}


/***********************************************/

void head93::init(TiXmlElement *xmlEle)
{
    mJsonV = "";
}

Json::Value head93::serialize()
{
    return mJsonV;
}
/***********************************************/

void headFe::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;
    const char *point = nullptr;

    tmpEle = xmlEle->FirstChildElement("head_src_node");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_node"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_node");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_node"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_send_sys");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            QString tmp(point);
            bool isOk;
            mJsonV["head_send_sys"] = tmp.toUInt(&isOk, 16);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_rcv_sys");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            QString tmp(point);
            bool isOk;
            mJsonV["head_rcv_sys"] = tmp.toUInt(&isOk, 16);
        }
    }
}
Json::Value headFe::serialize()
{
    return mJsonV;
}

/***********************************************/

void headJg::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;
    const char *point = nullptr;

    tmpEle = xmlEle->FirstChildElement("head_src_node");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_node"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_node");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_node"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_send_sys");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            QString tmp(point);
            bool isOk;
            mJsonV["head_send_sys"] = tmp.toUInt(&isOk, 16);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_rcv_sys");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            QString tmp(point);
            bool isOk;
            mJsonV["head_rcv_sys"] = tmp.toUInt(&isOk, 16);
        }
    }
}
Json::Value headJg::serialize()
{
    return mJsonV;
}

/***********************************************/

void headCzxk::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;
    const char *point = nullptr;

    tmpEle = xmlEle->FirstChildElement("head_src_node");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_src_node"] = std::atoi(point);
        }
    }

    tmpEle = xmlEle->FirstChildElement("head_dst_node");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["head_dst_node"] = std::atoi(point);
        }
    }
}
Json::Value headCzxk::serialize()
{
    return mJsonV;
}
/***********************************************/

value::value():
    mChangeModel(nullptr)
{

}

void value::init(TiXmlElement *xmlEle)
{
    TiXmlElement *tmpEle = nullptr;
    const char *point = nullptr;

    tmpEle = xmlEle->FirstChildElement("init");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
            mInitValue = std::string(point);
    }

    //随机仿真模型
    tmpEle = xmlEle->FirstChildElement("random");
    if(tmpEle)
    {
        mChangeModel = std::make_shared<RandaModel>();
        mChangeModel->init(tmpEle);
    }

    //直线模型
    tmpEle = xmlEle->FirstChildElement("line");
    if(tmpEle)
    {
        mChangeModel = std::make_shared<temperatureModel>();
        mChangeModel->init(tmpEle);
    }
}

std::string value::getValue()
{
    valueType v;

    if(mChangeModel != nullptr)
    {
        v = std::to_string(mChangeModel->value());
    }
    else
    {
        v = mInitValue;
    }
    return v;
}

/*****************************************/

frame::frame():
    mHeadObj(nullptr),
    mCurMsgType(GeneralMsg)
{

}

void frame::setIcdAdapter(Pf::PfIcdWorkBench::icdFrameAdapter *icdAdapter)
{
    //分别获取FE 93 BE帧解析
    if(icdAdapter == nullptr)
    {
        UT_THROW_EXCEPTION("icdAdapter == nullptr");
    }

    std::shared_ptr<Pf::PfIcdWorkBench::frameObj> tmpObj = nullptr;

    tmpObj = icdAdapter->getFrameObj(FRAME_93);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get 93 frame obj faild!");
    }
    mIcdFrameObj[FRAME_93] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_BE);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get BE frame obj faild!");
    }    
    mIcdFrameObj[FRAME_BE] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_FE);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get FE frame obj faild!");
    }
    mIcdFrameObj[FRAME_FE] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_MIDDLE);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get 中间件 frame obj faild!");
    }
    //modify 2020-5-9 18:19:40 中间件时需选择确认标志
    if(mHeadObj && FRAME_MIDDLE == mHeadObj->frameType())
        tmpObj->setAttribute(mHeadObj->serialize());

    //end
    mIcdFrameObj[FRAME_MIDDLE] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_XK);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get 信控协议 frame obj faild!");
    }
    mIcdFrameObj[FRAME_XK] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_DM);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get 定位瞄准协议 frame obj faild!");
    }
    mIcdFrameObj[FRAME_DM] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_CZXK);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get 车长显控通讯协议 frame obj faild!");
    }
    mIcdFrameObj[FRAME_CZXK] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_JG);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get 机柜协议 frame obj faild!");
    }
    mIcdFrameObj[FRAME_JG] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_C3);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get C3通讯协议 frame obj faild!");
    }
    mIcdFrameObj[FRAME_C3] = tmpObj;

    tmpObj = icdAdapter->getFrameObj(FRAME_M1553B);
    if(tmpObj == nullptr)
    {
        UT_THROW_EXCEPTION("get M1553B协议 frame obj faild!");
    }
    mIcdFrameObj[FRAME_M1553B] = tmpObj;
}

void frame::init(TiXmlElement *xmlEle)
{
    //获取帧头
    TiXmlElement *headEle = xmlEle->FirstChildElement("head");
    if(headEle)
    {        
        TiXmlElement *beEle = headEle->FirstChildElement(FRAME_BE);
        if(beEle)
        {
            mHeadObj = std::make_shared<headBe>();
            mHeadObj->init(beEle);

            mCurFrameType = FRAME_BE;
        }

        TiXmlElement *feEle = headEle->FirstChildElement(FRAME_FE);
        if(feEle)
        {
            mHeadObj = std::make_shared<headFe>();
            mHeadObj->init(feEle);

            mCurFrameType = FRAME_FE;
        }

        TiXmlElement *noEle = headEle->FirstChildElement(FRAME_93);
        if(noEle)
        {
            mHeadObj = std::make_shared<head93>();
            mHeadObj->init(noEle);

            mCurFrameType = FRAME_93;
        }

        TiXmlElement *middleEle = headEle->FirstChildElement(FRAME_MIDDLE);
        if(middleEle)
        {
            mHeadObj = std::make_shared<headMiddle>();
            mHeadObj->init(middleEle);

            mCurFrameType = FRAME_MIDDLE;
        }

        TiXmlElement *xkEle = headEle->FirstChildElement(FRAME_XK);
        if(xkEle)
        {
            //mHeadObj = std::make_shared<headMiddle>();
            //mHeadObj->init(middleEle);

            mCurFrameType = FRAME_XK;
        }

        TiXmlElement *dmEle = headEle->FirstChildElement(FRAME_DM);
        if(dmEle)
        {
            //mHeadObj = std::make_shared<headMiddle>();
            //mHeadObj->init(middleEle);

            mCurFrameType = FRAME_DM;
        }

        TiXmlElement *czxkEle = headEle->FirstChildElement(FRAME_CZXK);
        if(czxkEle)
        {
            mHeadObj = std::make_shared<headCzxk>();
            mHeadObj->init(czxkEle);

            mCurFrameType = FRAME_CZXK;
        }
        TiXmlElement *c3Ele = headEle->FirstChildElement(FRAME_C3);
        if(c3Ele)
        {
            //mHeadObj = std::make_shared<headMiddle>();
            //mHeadObj->init(middleEle);

            mCurFrameType = FRAME_C3;
        }

        TiXmlElement *jgEle = headEle->FirstChildElement(FRAME_JG);
        if(jgEle)
        {
            mHeadObj = std::make_shared<headJg>();
            mHeadObj->init(jgEle);

            mCurFrameType = FRAME_JG;
        }

        TiXmlElement *m1553BEle = headEle->FirstChildElement(FRAME_M1553B);
        if(m1553BEle)
        {
            mHeadObj = std::make_shared<head1553B>();
            mHeadObj->init(m1553BEle);

            mCurFrameType = FRAME_M1553B;
        }
    }


    //获取数据域
    TiXmlElement *dataFieldEle = xmlEle->FirstChildElement("datafields");
    if(dataFieldEle)
    {
        //获取命令
        TiXmlElement *cmdEle = dataFieldEle->FirstChildElement("cmd");
        if(cmdEle)
        {
            for (cmdEle; cmdEle; cmdEle = cmdEle->NextSiblingElement("cmd"))
            {
                TiXmlElement *tmpEle = nullptr;
                const char *point = nullptr;
                std::string tableStr = "";
                std::string codingStr = "";

                tmpEle = cmdEle->FirstChildElement("table");
                if(tmpEle)
                {
                    point = tmpEle->GetText();
                    if(point)
                    {
                        tableStr = std::string(point);

                        setCustomMsgFromTable(tableStr);
                    }
                }

                tmpEle = cmdEle->FirstChildElement("coding");
                if(tmpEle)
                {
                    point = tmpEle->GetText();
                    if(point)
                        codingStr = std::string(point);
                }

                mParamsVec.push_back(std::make_tuple(tableStr, codingStr, std::make_shared<value>("0")));

                mCurParamType = Cmd;
            }
        }

        //获取参数
        TiXmlElement *paramEle = dataFieldEle->FirstChildElement("param");
        if(paramEle)
        {
            for (paramEle; paramEle; paramEle = paramEle->NextSiblingElement("param"))
            {
                TiXmlElement *tmpEle = nullptr;
                const char *point = nullptr;
                value *tmpV = nullptr;
                std::string tableStr = "";
                std::string codingStr = "";

                tmpEle = paramEle->FirstChildElement("table");
                if(tmpEle)
                {
                    point = tmpEle->GetText();
                    if(point)
                    {
                        tableStr = std::string(point);
                        setCustomMsgFromTable(tableStr);
                    }
                }

                tmpEle = paramEle->FirstChildElement("coding");
                if(tmpEle)
                {
                    point = tmpEle->GetText();
                    if(point)
                        codingStr = std::string(point);
                }

                tmpEle = paramEle->FirstChildElement("value");
                if(tmpEle)
                {
                    tmpV = new value();
                    tmpV->init(tmpEle);
                }

                mParamsVec.push_back(std::make_tuple(tableStr, codingStr, std::shared_ptr<value>(tmpV)));

                mCurParamType = Param;
            }
        }
    }
}

bool frame::is1553B()
{
    if(!mHeadObj)
        return false;

    if(FRAME_M1553B != mHeadObj->frameType())
        return false;

    return true;
}

std::string frame::get1553BModel()
{
    std::string model = "";

    if(!is1553B())
        return model;

    Json::Value tmpJs = mHeadObj->serialize();
    if( tmpJs["model"].isNull() )
        return model;

    model = tmpJs["model"].asString();

    return model;
}

std::string frame::get1553BBus()
{
    std::string model = "";

    if(!is1553B())
        return model;

    Json::Value tmpJs = mHeadObj->serialize();
    if( tmpJs["bus"].isNull() )
        return model;

    model = tmpJs["bus"].asString();

    return model;
}


bool frame::getBcModelInfo(int &rtAddr, int &saAddr)
{
    if(!is1553B())
        return false;

    Json::Value tmpJs = mHeadObj->serialize();
    if(tmpJs["RT"].isNull() || tmpJs["SA"].isNull())
        return false;

    rtAddr = tmpJs["RT"].asInt();
    saAddr = tmpJs["SA"].asInt();

    return true;
}

bool frame::getRtModelInfo(int &sRt, int &sSa, int &rRt, int &rSa, int &size)
{
    if(!is1553B())
        return false;

    Json::Value tmpJs = mHeadObj->serialize();
    if(tmpJs["S_RT"].isNull() || tmpJs["S_SA"].isNull() || tmpJs["R_RT"].isNull() || tmpJs["R_SA"].isNull() || tmpJs["DATA_SIZE"].isNull())
        return false;

    sRt = tmpJs["S_RT"].asInt();
    sSa = tmpJs["S_SA"].asInt();

    rRt = tmpJs["R_RT"].asInt();
    rSa = tmpJs["R_SA"].asInt();

    size = tmpJs["DATA_SIZE"].asInt();

    return true;
}

void frame::setCustomMsgFromTable(std::string table)
{
    if(XK_CUSTOM_MSG == table)
    {
        mCurMsgType = CustomMsg;
    }
    else if(UI_CUSTOM_MSG == table)
    {
        mCurMsgType = CustomMsg;
    }
    else
    {
        mCurMsgType = GeneralMsg;
    }
}

void frame::getResendMsg(std::vector<unsigned char> &msg)
{
    auto findItor = mIcdFrameObj.find(mCurFrameType);
    if(findItor != mIcdFrameObj.end())
    {
        (findItor->second)->resendMsg(msg);
    }
}

std::shared_ptr<Pf::PfIcdWorkBench::frameObj> frame::getCurFrameObj()
{
    auto findItor = mIcdFrameObj.find(mCurFrameType);
    if(findItor != mIcdFrameObj.end())
    {
        return (findItor->second);
    }

    return nullptr;
}

void frame::getCustomMsg(std::string &table, std::string &coding)
{
    if(mParamsVec.size() <= 0)
        return ;

    table = std::get<Param_Table_Index>(mParamsVec.at(0));
    coding = std::get<Param_Coding_Index>(mParamsVec.at(0));
}

void frame::getFrameMsg(std::vector<unsigned char> &msg, bool &isAck, int resendCnt)
{
    if(mParamsVec.size() <= 0)
        return ;

    msg.clear();

    //获取表号，编码
    QString table = QString::fromStdString(std::get<Param_Table_Index>(mParamsVec.at(0)));
    QString coding = QString::fromStdString(std::get<Param_Coding_Index>(mParamsVec.at(0)));

    bool isOk;
    Json::Value info;
    paramsTable::getInstance()->getValue(table, coding, info);
    if(!info.isNull())
    {        
        std::string frameType = mCurFrameType;
        std::string infoWordType = info[PARAM_TABLE_INFO_WORD_TYPE].asString();
        std::string cmdType = info[PARAM_TABLE_CMD_TYPE].asString();

        std::string ackStr = info[PARAM_TABLE_IS_ACK].asString();
        if(IS_ACK_YES == ackStr)
            isAck = true;
        else if(IS_ACK_NO == ackStr)
            isAck = false;

        //获取域信息
        Json::Value regionJs = fill(frameType, infoWordType);

        //获取头信息
        if(mHeadObj)
        {
            regionJs["head"] = mHeadObj->serialize();

            if( (FRAME_BE == mHeadObj->frameType()) || (FRAME_MIDDLE == mHeadObj->frameType()))
            {
                //填充信息字类型字段
                regionJs["head"]["head_info_word_type"] = std::atoi(infoWordType.c_str());

                //填充帧类型
                if(CMD_TYPE_CMD == cmdType)
                {
                    regionJs["head"]["head_frame_type"] = 2;
                }
                else if(CMD_TYPE_STATE == cmdType)
                {
                    regionJs["head"]["head_frame_type"] = 1;
                }
                else if(CMD_TYPE_DATA == cmdType)
                {
                    regionJs["head"]["head_frame_type"] = 0;
                }

                regionJs["head"]["head_table"] = table.toInt(&isOk, 10);
            }
        }

#ifdef DEBUG_FRAME
         UT_SHOW(regionJs.toStyledString());
#endif

         //增加重传次数
         regionJs["resendCnt"] = resendCnt;

         //icd 仿真
        auto findItor = mIcdFrameObj.find(frameType);
        if(findItor != mIcdFrameObj.end())
        {
            (findItor->second)->setUuid(mCurUuid);
            (findItor->second)->simulation(msg, regionJs.toStyledString());
        }
        else
        {
#ifdef DEBUG_FRAME
            UT_SHOW("get ICD faild(" + frameType + ")");
#endif
        }
    }
}

Json::Value frame::fill(const std::string &frameType, const std::string &infoWordType)
{
    Json::Value regionJs;

    if( (frameType == FRAME_M1553B) || (frameType == FRAME_93) || (frameType == FRAME_XK) || (frameType == FRAME_DM) || (frameType == FRAME_CZXK)|| (frameType == FRAME_C3))
    {
        regionJs = fill93();
    }
    else if( (frameType == FRAME_FE) || (frameType == FRAME_JG))
    {
        regionJs = fillFe();
    }
    else if( (frameType == FRAME_BE))
    {
        regionJs = fillBe(infoWordType);
    }
    else if( (frameType == FRAME_MIDDLE))
    {
        regionJs = fillMiddle(infoWordType);
    }
    return regionJs;
}

Json::Value frame::fillRegion()
{
    Json::Value jsV;

    if(!(mParamsVec.size() > 0))
        return jsV;

    //获取表号
    std::string table = std::get<Param_Table_Index>( mParamsVec[0]);

    QString tmpTable = QString::fromStdString(table);

    bool isOk;
    jsV["table_num"] = tmpTable.toUInt(&isOk, 16);;

    Json::Value dataJs;


    Json::Value tmpJs;

    if(Cmd == mCurParamType) //指令则按需填充
    {
        for(auto params : mParamsVec)
        {
            tmpJs["coding"] = std::get<Param_Coding_Index>(params);
            tmpJs["value"] = std::atoi((std::get<Param_Value_Index>(params))->getValue().c_str());
        }
    }
    else //非指令则按数据库顺序进行填充（防止填充顺序不一致）
    {
        //从数据库中获取参数信息
        Json::Value paramValues;
        paramsTable::getInstance()->getValues(jsV["table_num"].asUInt(), paramValues);

        for(int index = 0; index < paramValues.size(); index++)
        {
            int coding = paramValues[index][PARAM_TABLE_CODING_NUM].asInt();
            Json::Value tmpJs;

            tmpJs["coding"] = coding;

            std::string dataType = paramValues[index][PARAM_TABLE_DATA_TYPE].asString();
            std::string initValue = paramValues[index][PARAM_TABLE_INIT_VALUE].asString();

            auto findItor = std::find_if(mParamsVec.begin(), mParamsVec.end(), [=](const paramsType &v){
                return (table == std::get<Param_Table_Index>(v)) && (std::to_string(coding) == std::get<Param_Coding_Index>(v));
            });

            if(findItor != mParamsVec.end())
            {
                initValue = std::get<Param_Value_Index>(*findItor)->getValue();
            }

            if( (Pf::PfIcdWorkBench::ieee32Type == dataType) || (Pf::PfIcdWorkBench::ieee64Type == dataType))
            {               
                tmpJs["value"] = std::atof(initValue.c_str());
            }
            else if( (Pf::PfIcdWorkBench::ncharType == dataType) || (Pf::PfIcdWorkBench::nRawType == dataType))
            {
                tmpJs["value"] = initValue;
            }
            else
            {
                QString tmpConvert = initValue.c_str();

                unsigned long tmpV = tmpConvert.toULong(&isOk, 10);

                if(!isOk)
                    tmpV = tmpConvert.toULong(&isOk, 16);

                if(!isOk)
                    tmpV = 0;

                if(!isOk)
                    tmpV = tmpConvert.toDouble(&isOk);

                tmpJs["value"] = (Json::UInt64)tmpV;
            }

            dataJs.append(tmpJs);
        }
    }


    jsV["data"] = dataJs;
    return jsV;
}

Json::Value frame::fill93()
{
    Json::Value frameJs;

    frameJs["region"] = fillRegion();

    return frameJs;
}

Json::Value frame::fillFe()
{
    Json::Value frameJs;

    frameJs["region"] = fillRegion();

    return frameJs;
}

Json::Value frame::getRunItems()
{
    Json::Value infoJs;

    for(auto param : mParamsVec)
    {
        Json::Value tmpJs;

        QString table = QString::fromStdString(std::get<Param_Table_Index>(param));
        QString coding = QString::fromStdString(std::get<Param_Coding_Index>(param));
        QString value = QString::fromStdString(std::get<Param_Value_Index>(param)->getValue());

        //获取名称
        Json::Value nameJs;
        if(paramsTable::getInstance()->getValue(table, coding, nameJs))
        {
            tmpJs["name"] = nameJs[PARAM_TABLE_PARAM_NAME].asString();

        }
        tmpJs["value"] = value.toStdString();
        tmpJs["table"] = table.toStdString();
        tmpJs["coding"] = coding.toStdString();

        infoJs.append(tmpJs);
    }

    return infoJs;
}

Json::Value frame::fillMiddle(const std::string &infoWord)
{
    Json::Value otherJs;
    Json::Value infoWordJs;

    if(!(mParamsVec.size() > 0))
        return infoWordJs;

    Json::Value headJs = mHeadObj->serialize();

    //获取弹编号、设备序号、模块序号、备用字符串
    int dNum = 0, devNum = 0, modelNum = 0, data = 0;
    std::string reserveStr = "";

    if(!headJs.isNull())
    {
        if(!headJs["head_d_num"].isNull())
        {
            dNum = headJs["head_d_num"].asInt();
        }

        if(!headJs["head_dev_num"].isNull())
        {
            devNum = headJs["head_dev_num"].asInt();
        }

        if(!headJs["head_modle_num"].isNull())
        {
            modelNum = headJs["head_modle_num"].asInt();
        }

        if(!headJs["head_reserve"].isNull())
        {
            reserveStr = headJs["head_reserve"].asString();
        }

        if(!headJs["head_data"].isNull())
        {
            data = headJs["head_data"].asInt();
        }
    }

    //获取表号
    std::string table = std::get<Param_Table_Index>( mParamsVec[0]);

    int infoWordType = std::atoi(infoWord.c_str());
    if(InfoWord_Two == infoWordType)    //信息字格式为2时需要填充 infoWord 及 region
    {
        //step1：填充infoWord信息
        int coding = std::atoi(std::get<Param_Coding_Index>(mParamsVec[0]).c_str());
        Json::Value tmpJs;

        tmpJs.append(fillMiddleInfoWord1(coding, dNum,  reserveStr));

        otherJs["infoWord"] = tmpJs;

        //step2：填充region信息
        otherJs["region"] = fillRegion();
    }
    else    //其它信息字只需填充infoWord
    {
        if(Cmd == mCurParamType) //指令则按需填充
        {
            for(auto params : mParamsVec)
            {
                int coding = std::atoi(std::get<Param_Coding_Index>(params).c_str());

                Json::Value tmpJs;

                if(InfoWord_One == infoWordType)
                {
                    tmpJs = fillMiddleInfoWord0(coding, dNum, 1, 4, Json::Value(data), false, reserveStr);
                }
                else if(InfoWord_Three == infoWordType)
                {
                    tmpJs = fillMiddleInfoWord2(coding, dNum, devNum, modelNum, 1, 4, Json::Value(data), false, reserveStr);
                }

                infoWordJs.append(tmpJs);
            }
        }
        else //非指令则按数据库顺序进行填充（防止填充顺序不一致）
        {
            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(table, paramValues);

            for(int index = 0; index < paramValues.size(); index++)
            {
                int coding = paramValues[index][PARAM_TABLE_CODING_NUM].asInt();

                double min = std::atof(paramValues[index][PARAM_TABLE_MIN_VALUE].asString().c_str());
                double max = std::atof(paramValues[index][PARAM_TABLE_MAX_VALUE].asString().c_str());

                std::string cmdType = paramValues[index][PARAM_TABLE_CMD_TYPE].asString();
                std::string dataType = paramValues[index][PARAM_TABLE_DATA_TYPE].asString();
                std::string initValue = paramValues[index][PARAM_TABLE_INIT_VALUE].asString();
                int sendDataType = 0;
                int sendDataLen = 0;
                Json::Value sendData;

                auto findItor = std::find_if(mParamsVec.begin(), mParamsVec.end(), [=](const paramsType &v){
                    return (table == std::get<Param_Table_Index>(v)) && (std::to_string(coding) == std::get<Param_Coding_Index>(v));
                });

                bool isOver = false;

                if(findItor != mParamsVec.end())
                {
                    initValue = std::get<Param_Value_Index>(*findItor)->getValue();
                }

                if(Pf::PfIcdWorkBench::ncharType == dataType)
                {
                    sendDataType = Pf::PfIcdWorkBench::String_Type;
                    sendDataLen = initValue.size();
                    sendData = initValue;
                }
                else if(Pf::PfIcdWorkBench::ieee64Type == dataType)
                {
                    sendDataType = Pf::PfIcdWorkBench::Ieee64_Type;
                    sendDataLen = 8;
                    sendData = (double)std::atof(initValue.c_str());
                }
                else if(Pf::PfIcdWorkBench::ieee32Type == dataType)
                {
                    sendDataType = Pf::PfIcdWorkBench::Ieee32_Type;
                    sendDataLen = 4;
                    sendData = (float)std::atof(initValue.c_str());
                }
                else if(Pf::PfIcdWorkBench::uint32Type == dataType)
                {
                    sendDataType = Pf::PfIcdWorkBench::Uint32_Type;
                    sendDataLen = 4;
                    sendData = std::atoi(initValue.c_str());
                }
                else
                {
                    sendDataType = Pf::PfIcdWorkBench::Int32_Type;
                    sendDataLen = 4;
                    sendData = std::atoi(initValue.c_str());
                }

                if(!(Pf::PfIcdWorkBench::ncharType == dataType))
                {
                    //判断是否超差
                    double v = std::atof(initValue.c_str());
                    if( (v >= min) && (v <= max))
                    {
                        isOver = false;
                    }
                    else
                    {
                        isOver = true;
                    }
                }

                Json::Value tmpJs;

                if(InfoWord_One == infoWordType)
                {
                    tmpJs = fillMiddleInfoWord0(coding, dNum, sendDataType, sendDataLen, sendData, isOver, reserveStr);
                }
                else if(InfoWord_Three == infoWordType)
                {
                    tmpJs = fillMiddleInfoWord2(coding, dNum, devNum, modelNum, sendDataType, sendDataLen, sendData, isOver, reserveStr);
                }

                infoWordJs.append(tmpJs);
            }
        }

        otherJs["infoWord"] = infoWordJs;
    }
    return otherJs;
}

Json::Value frame::fillMiddleInfoWord0(int coding, int num, int dataType, int dataLen, Json::Value data, bool isOver, std::string reserve)
{
    Json::Value value;

    value["info_1_code"] = coding;
    value["info_1_data_type"] = dataType;
    value["info_1_over"] = (int)isOver;
    value["info_1_num"] = num;
    value["info_1_data_len"] = dataLen;
    value["info_1_data"] = data;
    value["info_1_reserve_len"] = reserve.size();
    value["info_1_reserve"] = reserve;

    return value;
}

Json::Value frame::fillMiddleInfoWord1(int coding, int num, std::string reserve)
{
    Json::Value value;

    value["info_2_code"] = coding;
    value["info_2_num"] = num;
    value["info_2_reserve_len"] = reserve.size();
    value["info_2_reserve"] = reserve;

    return value;
}

Json::Value frame::fillMiddleInfoWord2(int coding, int dNum, int devNum, int modelNum, int dataType, int dataLen, Json::Value data, bool isOver, std::string reserve)
{
    Json::Value value;

    value["info_3_code"] = coding;
    value["info_3_dev_num"] = devNum;
    value["info_3_module"] = modelNum;
    value["info_3_data_type"] = dataType;
    value["info_3_over"] = (int)isOver;
    value["info_3_num"] = dNum;
    value["info_3_data_len"] = dataLen;
    value["info_3_data"] = data;
    value["info_3_reserve_len"] = reserve.size();
    value["info_3_reserve"] = reserve;

    return value;
}

Json::Value frame::fillBe(const std::string &infoWord)
{
    Json::Value otherJs;
    Json::Value infoWordJs;

    if(!(mParamsVec.size() > 0))
        return infoWordJs;

    int infoWordType = std::atoi(infoWord.c_str());

    //获取弹编号、设备序号、模块序号、备用字符串
    int dNum = 0, devNum = 0, modelNum = 0, data = 0;
    Json::Value headJs = mHeadObj->serialize();

    if(!headJs.isNull())
    {
        if(!headJs["head_d_num"].isNull())
        {
            dNum = headJs["head_d_num"].asInt();
        }

        if(!headJs["head_dev_num"].isNull())
        {
            devNum = headJs["head_dev_num"].asInt();
        }

        if(!headJs["head_modle_num"].isNull())
        {
            modelNum = headJs["head_modle_num"].asInt();
        }

        if(!headJs["head_data"].isNull())
        {
            data = headJs["head_data"].asInt();
        }
    }


    //获取表号
    std::string table = std::get<Param_Table_Index>( mParamsVec[0]);

    if(InfoWord_Two == infoWordType)    //信息字格式为2时需要填充 infoWord 及 region
    {
        //step1：填充infoWord信息
        int coding = std::atoi(std::get<Param_Coding_Index>(mParamsVec[0]).c_str());
        Json::Value tmpJs;
        tmpJs.append(fillInfoTwo(std::atoi(table.c_str()), coding));

        otherJs["infoWord"] = tmpJs;

        //step2：填充region信息
        otherJs["region"] = fillRegion();
    }
    else    //其它信息字只需填充infoWord
    {
        if(Cmd == mCurParamType) //指令则按需填充
        {
            for(auto params : mParamsVec)
            {
                int coding = std::atoi(std::get<Param_Coding_Index>(params).c_str());

                Json::Value tmpJs;

                if(InfoWord_One == infoWordType)
                {
                    tmpJs = fillInfoOne(std::atoi(table.c_str()), coding, Json::Value(data), 2, false, dNum);
                }
                else if(InfoWord_Three == infoWordType)
                {
                    tmpJs = fillInfoThree(std::atoi(table.c_str()), coding, Json::Value(data), Pf::PfIcdWorkBench::intType, false, devNum, modelNum);
                }

                infoWordJs.append(tmpJs);
            }
        }
        else //非指令则按数据库顺序进行填充（防止填充顺序不一致）
        {
            //从数据库中获取参数信息
            Json::Value paramValues;
            paramsTable::getInstance()->getValues(table, paramValues);

            for(int index = 0; index < paramValues.size(); index++)
            {
                int coding = paramValues[index][PARAM_TABLE_CODING_NUM].asInt();

                double min = std::atof(paramValues[index][PARAM_TABLE_MIN_VALUE].asString().c_str());
                double max = std::atof(paramValues[index][PARAM_TABLE_MAX_VALUE].asString().c_str());

                std::string cmdType = paramValues[index][PARAM_TABLE_CMD_TYPE].asString();
                std::string dataType = paramValues[index][PARAM_TABLE_DATA_TYPE].asString();
                std::string initValue = paramValues[index][PARAM_TABLE_INIT_VALUE].asString();

                auto findItor = std::find_if(mParamsVec.begin(), mParamsVec.end(), [=](const paramsType &v){
                    return (table == std::get<Param_Table_Index>(v)) && (std::to_string(coding) == std::get<Param_Coding_Index>(v));
                });

                bool isOver = false;

                if(findItor != mParamsVec.end())
                {
                    initValue = std::get<Param_Value_Index>(*findItor)->getValue();
                }

                if(!(Pf::PfIcdWorkBench::ncharType == dataType))
                {
                    //判断是否超差
                    double v = std::atof(initValue.c_str());
                    if( (v >= min) && (v <= max))
                    {
                        isOver = false;
                    }
                    else
                    {
                        isOver = true;
                    }
                }

                Json::Value tmpJs;
                int sendDataType = 0;
                int sendDataLen = 0;
                Json::Value sendData;

                if( (Pf::PfIcdWorkBench::ieee64Type == dataType) || (Pf::PfIcdWorkBench::ieee32Type == dataType))
                {
                    sendData = (float)std::atof(initValue.c_str());
                    sendDataType = 1;
                }
                else if( (Pf::PfIcdWorkBench::ncharType == dataType) || (Pf::PfIcdWorkBench::nRawType == dataType))
                {
                    sendData = initValue;
                }
                else
                {
                    sendData = std::atoi(initValue.c_str());
                    sendDataType = 2;
                }

                if(InfoWord_One == infoWordType)
                {
                    tmpJs = fillInfoOne(std::atoi(table.c_str()), coding, sendData, sendDataType, isOver, dNum);
                }
                else if(InfoWord_Three == infoWordType)
                {
                    tmpJs = fillInfoThree(std::atoi(table.c_str()), coding, sendData, dataType, isOver, 0, dNum);
                }

                infoWordJs.append(tmpJs);
            }
        }

        otherJs["infoWord"] = infoWordJs;
    }

    return otherJs;
}

Json::Value frame::fillInfoOne(const unsigned short &table, const unsigned short &coding, const Json::Value &data, const int &dataType, const bool &isOver, const unsigned short &num)
{
    Json::Value value;

    value["info_1_table_num"] = table;
    value["info_1_code"] = coding;
    value["info_1_data"] = data;
    value["info_1_data_type"] = dataType;
    value["info_1_over"] = (int)isOver;
    value["info_1_num"] = num;
    value["info_1_reserve"] = 0;

    return value;
}

Json::Value frame::fillInfoTwo(const unsigned short &table, const unsigned short &coding)
{
    Json::Value value;

    value["info_2_table_num"] = table;
    value["info_2_code"] = coding;
    value["info_2_reserve"] = 0;

    return value;
}

Json::Value frame::fillInfoThree(int table, int coding, const Json::Value &data, std::string dataType, bool isOver, int devNum, int modelNum)
{
    Json::Value value;

    value["info_3_table_num"] = table;
    value["info_3_code"] = coding;
    value["info_3_dev_num"] = devNum;
    value["info_3_module"] = modelNum;

    //判断数值类型
    if( (Pf::PfIcdWorkBench::ieee32Type == dataType) || (Pf::PfIcdWorkBench::ieee64Type == dataType))
    {
        value["info_3_data_type"] = 1;
    }
    else if(Pf::PfIcdWorkBench::ncharType == dataType)
    {
        value["info_3_data_type"] = 3;
    }
    else
    {
        value["info_3_data_type"] = 2;
    }

    value["info_3_over"] = (int)isOver;

    if(Pf::PfIcdWorkBench::ncharType == dataType) //字符串类型
    {
        value["info_3_data_value"] = 0;
    }
    else
    {
        if( (Pf::PfIcdWorkBench::ieee32Type == dataType) || (Pf::PfIcdWorkBench::ieee64Type == dataType))
            value["info_3_data_value"] = data.asFloat();
        else
            value["info_3_data_value"] = data.asInt();
    }
    value["info_3_reserve"] = 0;

    if(Pf::PfIcdWorkBench::ncharType == dataType)
    {
        value["info_3_string_len"] = data.asString().size();
        value["info_3_data_value"] = data.asString();
    }

    return value;
}
