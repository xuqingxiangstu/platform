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

    std::cout << "exit" << std::endl;
    return a.exec();
}
