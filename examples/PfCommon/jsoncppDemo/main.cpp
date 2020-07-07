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

    double s1 = 4.5;
    int s2 = 4;
    unsigned int s3 = 0x10;
    float s4 = 3.5;

    Json::Value t1 = Json::Value(s1);
    Json::Value t2 = Json::Value(s2);
    Json::Value t3 = Json::Value(s3);
    Json::Value t4 = Json::Value(s4);

    bool res = false;

    res = t1.isDouble();
    qDebug() << res;
    res = t1.isInt();
    qDebug() << res;
    res = t1.isInt64();
    qDebug() << res;
    res = t1.isIntegral();
    qDebug() << res;

    res = t2.isDouble();
    qDebug() << res;
    res = t2.isInt();
    qDebug() << res;
    res = t2.isInt64();
    qDebug() << res;
    res = t2.isIntegral();
    qDebug() << res;

    res = t3.isDouble();
    qDebug() << res;
    res = t3.isInt();
    qDebug() << res;
    res = t3.isInt64();
    qDebug() << res;
    res = t3.isIntegral();
    qDebug() << res;

    res = t4.isDouble();
    qDebug() << res;
    res = t4.isInt();
    qDebug() << res;
    res = t4.isInt64();
    qDebug() << res;
    res = t4.isIntegral();
    qDebug() << res;

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
