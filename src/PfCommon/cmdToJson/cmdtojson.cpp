#include "cmdtojson.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include "../jsoncpp/json.h"
namespace Pf
{
    namespace PfCommon
    {
        cmdToJson::cmdToJson()
        {

        }

        ///关键信息显示
        std::string cmdToJson::keyShowCmd(const std::vector<std::string> &ids)
        {
            /*
                {
                    "msgType":"keyParams_show",
                    "msg":{
                            "params":"P1;P2"
                    }
                }
             */
            Json::Value root;
            Json::Value item;
            std::string str;
            for(int i = 0; i < ids.size(); i ++)
            {
                str +=ids[i]+";";
            }
            str = str.substr(0, str.length() - 1);
            item["params"] = str;
            root["msgType"] = "keyParams_show";
            root["msg"] = item;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            //std::string out = root.asString();
            return out;

        }
        ///发送RS422数据
        std::string cmdToJson::sendRs422Cmd(const std::string &id,const std::vector<unsigned char> &data)
        {
            /*
                {
                    "msgType":"RS422",
                    "msg":{
                            "id":"",
                            "data":[
                                    12,
                                    13,
                                    14
                            ]
                    }
                }
            */

            Json::Value root;
            Json::Value item;
            Json::Value arrayObj;
            for(int i=0;i<data.size();i++){
                arrayObj.append(data[i]);
            }
            item["id"] = id;

            item["data"] = arrayObj;
            root["msgType"] = "RS422";
            root["msg"] = item;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;

        }
        ///RS422回复结果
        std::string cmdToJson::replyRs422ResCmd(const std::string &result)
        {
            /*
             {
                "msgType":"RS422",
                "msg":{
                        "result":"errocode"
                }
              }
             */
            Json::Value root;
            Json::Value item;
            item["result"] = result;
            root["msgType"] = "RS422";
            root["msg"] = item;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }
        ///错误信息回复
        std::string cmdToJson::replyErrorCmd(const std::string &errmsg)
        {
            /*
                {
                    "msgType":"error",
                    "errmsg":"msg"
                }
             */

            Json::Value root;
            root["msgType"] = "error";
            root["errmsg"] = errmsg;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }
        ///开关量输入
        std::string cmdToJson::replySwitchInCmd(const std::vector <std::tuple<std::string,int,bool>> &msg)
        {
            /*
            {
                "msgType": "switch_in",
                "msg": [
                        {
                            "id": "e",
                            "channel": 1,
                            "status": true
                        }
                ]
            }
            */
            Json::Value root;
            Json::Value item;
            Json::Value arrayObj;
            for(int i = 0;i<msg.size();i++)
            {
                item["id"] = std::get<0>(msg[i]);
                item["channel"] = std::get<1>(msg[i]);
                item["status"] = std::get<2>(msg[i]);
                arrayObj.append(item);
            }
            root["msgType"] = "switch_in";
            root["msg"] = arrayObj;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }
        ///模拟量采集接口
        std::string cmdToJson::replyAnalogInCmd(const std::string id[],std::vector<double> value[],int size)
        {
            /*
            {
                "msgType":"analog_in",
                "msg":[
                    {
                        "id":"e",
                        "value":[
                                        3.4,
                                        4.5
                        ]
                    },
                    {
                        "id":"e1",
                        "value":[
                                        3.4,
                                        4.5
                        ]
                    }
                ]
            }
            */
            Json::Value root;
            Json::Value item;
            Json::Value arrayObj;

            for(int i = 0;i<size;i++)
            {
                Json::Value brrayObj;
                item["id"] = id[i];
                std::vector<double> temp;
                temp = value[i];
                for(int j = 0;j<temp.size();j++){
                    brrayObj.append(value[i][j]);
                }
                item["value"] = brrayObj;
                arrayObj.append(item);
            }
            root["msgType"] = "analog_in";
            root["msg"] = arrayObj;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }
        ///开关量输出接口
        std::string cmdToJson::sendSwitchOutCmd(const std::vector <std::tuple<std::string,int,bool>> &msg)
        {
            /*
            {
                "msgType": "switch_out",
                "msg":[
                    {
                        "id": "e",
                        "channel": 1,
                        "status": true
                    }
                ]
            }
            */
            Json::Value root;
            Json::Value item;
            Json::Value arrayObj;

            for(int i = 0;i<msg.size();i++)
            {
                item["id"] = std::get<0>(msg[i]);
                item["channel"] = std::get<1>(msg[i]);
                item["status"] = std::get<2>(msg[i]);
                arrayObj.append(item);
            }
            root["msgType"] = "switch_out";
            root["msg"] = arrayObj;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }
        ///模拟量输出接口
        std::string cmdToJson::sendAnalogOutCmd(const std::vector <std::tuple<std::string,int,double,float>> &msg)
        {
        /*
             {
                "msgType": "analog_out",
                "msg": [
                    {
                        "id": "e",
                        "channel": 1,
                        "min": 1.78,
                        "max": 2.3
                    },
                    {
                        "id": "e",
                        "channel": 1,
                        "min": 1.78,
                        "max": 2.3
                    }
                ]
             }
         */

            Json::Value root;
            Json::Value item;
            Json::Value arrayObj;

            for(int i = 0;i<msg.size();i++)
            {
                item["id"] = std::get<0>(msg[i]);
                item["channel"] = std::get<1>(msg[i]);
                item["min"] = std::get<2>(msg[i]);
                item["max"] = std::get<3>(msg[i]);
                arrayObj.append(item);
            }
            root["msgType"] = "analog_out";
            root["msg"] = arrayObj;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }

        std::string cmdToJson::sendCtrlCmd(const std::vector<std::string> &msg)
        {
            Json::Value root;
            Json::Value item;
            if(msg[0] == "init")
            {
                item["type"] = "init";

            }else if(msg[0] == "start")
            {
                item["type"] = "start";
                item["flow"] = msg[1];
                item["items"] = msg[2];

            }else if(msg[0] == "stop")
            {
                item["type"] = "stop";
                item["flow"] = msg[1];

            }else if(msg[0] == "suspend")
            {
                item["type"] = "suspend";
                item["flow"] = msg[1];

            }else if(msg[0] == "exit")
            {
                item["type"] = "exit";
                item["flow"] = msg[1];

            }else if(msg[0] == "continue")
            {
                item["type"] = "continue";
                item["flow"] = msg[1];

            }else if(msg[0] == "get_runitems")
            {
                item["type"] = "get_runitems";
                item["flow"] = msg[1];

            }else if(msg[0] == "load")
            {
                item["type"] = "load";
                item["flow"] = msg[1];
                item["path"] = msg[2];
            }
            root["msgType"] = "cmd";
            root["msg"] = item;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }

        std::string cmdToJson::replyCtrlCmd(const std::vector<std::string> &msg,bool param)
        {
            Json::Value root;
            Json::Value item;
            if(msg[0] == "init")
            {
                item["type"] = "init";
                item["result"] = msg[1];

            }else if(msg[0] == "exit")
            {
                item["type"] = "exit";
                item["result"] = msg[1];

            }else if(msg[0] == "start")
            {
                item["type"] = "start";
                item["flow"] = msg[1];
                item["result"] = msg[2];

            }else if(msg[0] == "load")
            {
                item["type"] = "load";
                item["flow"] = msg[1];
                item["result"] = msg[2];

            }else if(msg[0] == "stop")
            {
                item["type"] = "stop";
                item["flow"] = msg[1];
                item["result"] = msg[2];
            }else if(msg[0] == "suspend")
            {
                item["type"] = "suspend";
                item["flow"] = msg[1];
                item["result"] = msg[2];
            }else if(msg[0] == "continue")
            {
                item["type"] = "continue";
                item["flow"] = msg[1];
                item["result"] = msg[2];
            }else if(msg[0] == "get_runitems")
            {
                item["type"] = "get_runitems";
                item["flow"] = msg[1];
                item["result"] = msg[2];
                item["items"] = msg[3];
            }else if(msg[0] == "over")
            {
                item["type"] = "over";
                item["flow"] = msg[1];
                item["param"] = param;

            }else if(msg[0] == "showMsg")
            {
                item["type"] = "showMsg";
                item["flow"] = msg[1];
                item["status"] = param;
                item["show"] = msg[2];

            }else if(msg[0] == "switch_out")
            {
                item["type"] = "switch_out";
                item["result"]= msg[1];
            }else if(msg[0]== "analog_out")
            {
                item["type"] = "analog_out";
                item["result"]= msg[1];
            }
            root["msgType"] = "cmd";
            root["msg"] = item;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }

        std::string cmdToJson::replyTestStatusCmd(const std::string &flow,const std::tuple<std::string ,int ,std::string > &param)
        {
            /*
            ///测试状态 status （ test => 测试中、 normal => 测试正常、error => 测试异常、其它测试信息）

            {
                    "msgType":"cmd",
                    "msg":{
                                "type":"testStatus",
                                "flow":"normal",
                                "param":{
                                            "id":"",
                                            "row":0,
                                            "status":"test"
                                }
                    }
            }
            */
            Json::Value root;
            Json::Value item;
            Json::Value params;

            item["flow"] = flow;
            item["type"] = "testStatus";

            params["id"] = std::get<0>(param);
            params["row"] = std::get<1>(param);
            params["status"] = std::get<2>(param);

            item["param"] = params;
            root["msgType"] = "cmd";
            root["msg"] = item;
            root.toStyledString();
            std::string out = Json::FastWriter().write(root);
            return out;
        }
    }
}
