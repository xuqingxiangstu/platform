#include <QCoreApplication>

#include <iostream>
#include <QDebug>

#include "../../../src/PfCommon/jsoncpp/json.h"

/**
 * @brief json序列化反序列化例子
 * @example PfCommon/jsoncppDemo/main.cpp
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //序列化
    qDebug() << "序列化测试";

    Json::Value test;
    test["test"] = "56";
    test["state"] = true;

    std::cout << test.toStyledString() << std::endl;

    qDebug() << "反序列化测试";
#if 0
    //反序列化
    std::string json = "{\
                       \"head\":\
                       {\
                           \"head_frame_type\":9,\
                           \"head_src_sys_type\":10,\
                           \"head_src_sys_code\":11,\
                           \"head_src_node_code\":23,\
                           \"head_dst_sys_type\":10,\
                           \"head_dst_sys_code\":11,\
                           \"head_dst_node_code\":23,\
                           \"head_info_word_type\":2\
                       }}";

    Json::Reader reader;
    Json::Value root;
    if(reader.parse(json, root))
    {
        Json::Value head = root["head"];

        Json::Value::Members mem = head.getMemberNames();
        for (auto iter = mem.begin(); iter != mem.end(); iter++)
        {
            std::cout<<*iter<<" : " << head[*iter].asString();
        }
    }
#else
    std::string json = "{\
            \"table_num\":16896,\
            \"data\":\
            [\
                {\
                    \"coding\":1,\
                    \"value\":\"11\"\
                },\
                {\
                    \"coding\":2,\
                    \"value\":\"123\"\
                },\
                {\
                    \"coding\":3,\
                    \"value\":\"测试字符串\"\
                },\
                {\
                    \"coding\":4,\
                    \"value\":\"89\"\
                },\
                {\
                    \"coding\":5,\
                    \"value\":\"审批通过\"\
                }\
            ]\
        }\
";
    Json::Reader reader;
    Json::Value root;
    if(reader.parse(json, root))
    {
        Json::Value head = root["data"];
        for(auto v : head)
        {
            if(v["coding"] == 2)
            {
                std::cout << "find";
            }
        }
        Json::Value v;
        /*v["coding"] = 2;
        Json::Value fV = head.get(head.toStyledString().data(), head.toStyledString().data() + head.toStyledString().length(), v);
        std::cout << fV.toStyledString();*/
    }
#endif
    std::cout << "exit" << std::endl;
    return a.exec();
}
