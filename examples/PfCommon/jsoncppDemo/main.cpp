#include <QCoreApplication>
#include <iostream>

#include "../../../src/PfCommon/jsoncpp/json.h"

/**
 * @brief json序列化反序列化例子
 * @example PfCommon/jsoncppDemo/main.cpp
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //序列化
    Json::Value test;
    test["test"] = "56";
    test["state"] = true;

    std::cout << test.toStyledString() << std::endl;

    //反序列化
    std::string json = "{\"test\":\"56\",\"state\":true}";

    Json::Reader reader;
    Json::Value root;
    if(reader.parse(json, root))
    {
        std::cout << "test:" << root["test"].asString() << std::endl;
        std::cout << "state:" << root["state"].asBool() << std::endl;

    }

    std::cout << "exit" << std::endl;
    return a.exec();
}
