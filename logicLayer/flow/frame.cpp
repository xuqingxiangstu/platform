#include "frame.h"

#include "../../src/PfSql/paramsTable/paramstable.h"
#include "../../src/PfSmlModel/randaModel/randamodel.h"
#include "../../src/PfSmlModel/temperatureModel/temperatureModel.h"
#include "../../src/PfIcdWorkBench/icdData/datatype.h"

#include "../../src/PfCommon/tools/ut_error.h"

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
}

Json::Value headBe::serialize()
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

    tmpEle = xmlEle->FirstChildElement("data_type");
    if(tmpEle)
    {
        point = tmpEle->GetText();
        if(point)
        {
            mJsonV["data_type"] = std::atoi(point);
        }
    }
}

Json::Value headFe::serialize()
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
    mHeadObj(nullptr)
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
                        tableStr = std::string(point);
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

void frame::getResendMsg(std::vector<unsigned char> &msg)
{
    auto findItor = mIcdFrameObj.find(mCurFrameType);
    if(findItor != mIcdFrameObj.end())
    {
        (findItor->second)->resendMsg(msg);
    }
}

void frame::getFrameMsg(std::vector<unsigned char> &msg, bool &isAck, int resendCnt)
{
    if(mParamsVec.size() <= 0)
        return ;

    msg.clear();

    //获取表号，编码
    std::string table = std::get<Param_Table_Index>(mParamsVec.at(0));
    std::string coding = std::get<Param_Coding_Index>(mParamsVec.at(0));

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
            if(FRAME_BE == mHeadObj->frameType())
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
            (findItor->second)->simulation(msg, regionJs.toStyledString());
        }
        else
        {
            UT_SHOW("获取帧ICD失败(" + frameType + ")");
        }
    }
}

Json::Value frame::fill(const std::string &frameType, const std::string &infoWordType)
{
    Json::Value regionJs;

    if(frameType == FRAME_93)
    {
        regionJs = fill93();
    }
    else if(frameType == FRAME_FE)
    {
        regionJs = fillFe();
    }
    else if(frameType == FRAME_BE)
    {
        regionJs = fillBe(infoWordType);
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

    jsV["table_num"] = std::atoi(table.c_str());


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
        paramsTable::getInstance()->getValues(table, paramValues);



        for(int index = 0; index < paramValues.size(); index++)
        {
            int coding = paramValues[index][PARAM_TABLE_CODING_NUM].asInt();
            Json::Value tmpJs;

            tmpJs["coding"] = coding;

            std::string dataType = paramValues[index][PARAM_TABLE_DATA_TYPE].asString();
            std::string initValue = paramValues[index][PARAM_TABLE_PARAM_BIT_SIZE].asString();

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
            else if(Pf::PfIcdWorkBench::ncharType == dataType)
            {
                tmpJs["value"] = initValue;
            }
            else
            {
                tmpJs["value"] = std::atoi(initValue.c_str());
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

        std::string table = std::get<Param_Table_Index>(param);
        std::string coding = std::get<Param_Coding_Index>(param);
        std::string value = std::get<Param_Value_Index>(param)->getValue();

        //获取名称
        Json::Value nameJs;
        if(paramsTable::getInstance()->getValue(table, coding, nameJs))
        {
            tmpJs["name"] = nameJs[PARAM_TABLE_PARAM_NAME].asString();

        }
        tmpJs["value"] = value;
        tmpJs["table"] = table;
        tmpJs["coding"] = coding;

        infoJs.append(tmpJs);
    }

    return infoJs;
}

Json::Value frame::fillBe(const std::string &infoWord)
{
    Json::Value otherJs;
    Json::Value infoWordJs;

    if(!(mParamsVec.size() > 0))
        return infoWordJs;

    int infoWordType = std::atoi(infoWord.c_str());

    //获取表号
    std::string table = std::get<Param_Table_Index>( mParamsVec[0]);

    if(InfoWord_Two == infoWordType)    //信息字格式为2时需要填充 infoWord 及 region
    {
        //step1：填充infoWord信息
        int coding = std::atoi(std::get<Param_Coding_Index>(mParamsVec[0]).c_str());
        otherJs["infoWord"] = fillInfoTwo(std::atoi(table.c_str()), coding);

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
                    tmpJs = fillInfoOne(std::atoi(table.c_str()), coding, 0, false, 0);
                }
                else if(InfoWord_Three == infoWordType)
                {
                    tmpJs = fillInfoThree(std::atoi(table.c_str()), coding, "0", CMD_TYPE_DATA, false, 0, 0);
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
                std::string initValue = paramValues[index][PARAM_TABLE_PARAM_BIT_SIZE].asString();

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

                if(InfoWord_One == infoWordType)
                {
                    tmpJs = fillInfoOne(std::atoi(table.c_str()), coding, std::atof(initValue.c_str()), isOver, 0);
                }
                else if(InfoWord_Three == infoWordType)
                {
                    tmpJs = fillInfoThree(std::atoi(table.c_str()), coding, initValue, cmdType, isOver, 0, 0);
                }

                infoWordJs.append(tmpJs);
            }
        }

        otherJs["infoWord"] = infoWordJs;
    }

    return otherJs;
}

Json::Value frame::fillInfoOne(const unsigned short &table, const unsigned short &coding, const double &v, const bool &isOver, const unsigned short &num)
{
    Json::Value value;

    value["info_1_table_num"] = table;
    value["info_1_code"] = coding;
    value["info_1_data"] = v;
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

Json::Value frame::fillInfoThree(int table, int coding, std::string v, std::string dataType, bool isOver, int devNum, int modelNum)
{
    Json::Value value;

    value["info_3_table_num"] = table;
    value["info_3_code"] = coding;
    value["info_3_dev_num"] = devNum;
    value["info_3_module"] = modelNum;

    //判断数值类型
    if(CMD_TYPE_DATA == dataType)
    {
        value["info_3_data_type"] = 1;
    }
    else if(CMD_TYPE_STATE == dataType)
    {
        value["info_3_data_type"] = 2;
    }
    else if(CMD_CHAR_TYPE == dataType)
    {
        value["info_3_data_type"] = 3;
    }

    value["info_3_over"] = (int)isOver;

   if(CMD_CHAR_TYPE == dataType) //字符串类型
    {
        value["info_3_data_value"] = 0;
    }
    else //数值/状态
    {
        value["info_3_data_value"] = std::atof(v.c_str());
    }
    value["info_3_reserve"] = 0;

    if(CMD_CHAR_TYPE == dataType)
    {
        value["info_3_string_len"] = v.size();
        value["info_3_string_value"] = v;
    }

    return value;
}
