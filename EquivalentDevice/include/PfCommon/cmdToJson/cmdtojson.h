#ifndef CMDTOJSON_H
#define CMDTOJSON_H
/**
* @file cmdtojson.h
* @author ygt
* @brief
* @version 0.1
* @date 2020-2-17
* @copyright Copyright (c) 2020
* @todo json序列化
*/
#include<string>
#include<vector>
#include<tuple>
#include "cmdtojson_global.h"
/**
* @brief The cmdToJson class
* json序列化
*/
namespace Pf
{
    namespace PfCommon
    {
        class CMDTOJSONSHARED_EXPORT cmdToJson
        {

        public:
            cmdToJson();
        public:
            /**
             * @brief keyShowCmd    关键参数显示指令
             * @param ids 参数ID集合
             * @return json序列化
             */
            static std::string keyShowCmd(const std::vector<std::string> &ids);
            /**
             * @brief sendRs422Cmd    发送RS422数据
             * @param id 参数ID集合
             * @param data 数据data集合
             * @return json序列化
             */
            static std::string sendRs422Cmd(const std::string &id, const std::vector<unsigned char> &data);
            /**
             * @brief replyRs422ResCmd    RS422回复结果
             * @param result 回复结果
             * @return json序列化
             */
            static std::string replyRs422ResCmd(const std::string &result);
            /**
             * @brief replyErrorCmd    错误信息回复
             * @param errmsg 错误信息
             * @return json序列化
             */
            static std::string replyErrorCmd(const std::string &errmsg);
            /**
             * @brief replySwitchInCmd    开关量输入
             * @param msg(id,channel,status) 开关量参数集合
             * @return json序列化
             */
            static std::string replySwitchInCmd(const std::vector <std::tuple<std::string,int,bool>> &msg);
            /**
             * @brief replyAnalogInCmd    模拟量采集接口
             * @param id 参数id集合
             * @param value 参数value集合
             * @param size 参数size集合
             * @return json序列化
             */
            static std::string replyAnalogInCmd(const std::string id[],std::vector<double> value[],int size);
            /**
             * @brief sendSwitchOutCmd    开关量输出接口
             * @param msg(id,channel,status) 开关量参数集合
             * @return json序列化
             */
            static std::string sendSwitchOutCmd(const std::vector <std::tuple<std::string,int,bool>> &msg);
            /**
             * @brief sendAnalogOutCmd    模拟量输出接口
             * @param msg(id,channel,min,max) 模拟量参数集合
             * @return json序列化
             */
            static std::string sendAnalogOutCmd(const std::vector <std::tuple<std::string,int,double,float>> &msg);
            /**
             *函数为发送控制命令交互
             * msg中存放数据依次为type、flow、item或path
             * 例如 1.当应用层(也就是界面)想要初始化只需往参数msg里放"init"
             *      2.当应用层(也就是界面)想要运行测试时，需往参数msg里放"start"、“normal”、“all”
             *      3.当应用层(也就是界面)想要加载流程时，需往参数msg里放"load"、“normal”、“./cfgfile/test.xml”
             * 使用只需要赋值必要的参数即可
             */
            /**
             * @brief sendCtrlCmd    发送控制命令交互
             * @param msg(type,flow,item/path) 发送控制命令交互参数集合
             * @return json序列化
             */
            static std::string sendCtrlCmd(const std::vector<std::string> &msg);
            /**
             *函数回复为控制命令交互
             * msg中存放数据依次为type、result或flow、result或items或param
             * 例如 1.当向应用层(也就是界面)反馈初始化结果，只需往参数msg里放"init"、"errocode"
             *      2.当向应用层(也就是界面)反馈执行结果，需往参数msg里放"start"、“normal”、“errocode”
             *      3.当向应用层(也就是界面)某项的测试状态时，需往参数msg里放"get_runitems"、"normal"、"resInfo"、"items"
             * 使用只需要赋值必要的参数即可
             */
            /**
             * @brief replyCtrlCmd    回复控制命令交互
             * @param msg(type、result/flow、result/items/param) 回复控制命令交互参数集合
             * @return json序列化
             */
            static std::string replyCtrlCmd(const std::vector<std::string> &msg,bool param=false);
            /**
             * @brief replyTestStatusCmd    回复测试状态交互
             * @param flow 回复测试状态交互flow集合
             * @param param 回复测试状态交互param集合
             * @return json序列化
             */
            static std::string replyTestStatusCmd(const std::string &flow,const std::tuple<std::string ,int ,std::string > &param);

        };
    }
}

#endif // CMDTOJSON_H
