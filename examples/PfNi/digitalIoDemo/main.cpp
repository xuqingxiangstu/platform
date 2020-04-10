#include <QCoreApplication>

#include "digitalio.h"

#include <memory>
#include <iostream>

/**
 * @brief 数字量输入/输出测试
 * @example PfNi/digitalIoDemo/main.cpp
 */

using namespace Pf;

void testDi()
{
    //step1：实例化
    std::shared_ptr<PfNi::DigitalIn> obj = std::make_shared<PfNi::DigitalIn>();

    //step2：打开板卡

    std::string cardName = "PXI6528";   //板卡名称

    std::vector<int> ports; //板卡输入对应port号(0~2 共24路)
    ports.push_back(0);
    ports.push_back(1);
    ports.push_back(2);

    obj->initCard(cardName, ports);

    //step3：读取状态
    std::vector<unsigned char> values;  //状态

    if(obj->readValue(values))
    {
        for(int index = 0; index < values.size(); index++)
        {
            unsigned char v = values.at(index);
            for(int chIndex = 0; chIndex < 8; chIndex++)
            {
                std::cout << "port(" << std::to_string(index) << ") channel(" << std::to_string(chIndex) << ") ";
                if((v >> chIndex) & 1)
                    std::cout <<  std::string("高");
                else
                    std::cout << std::string("低");
                std::cout << std::endl;
            }
        }
    }
    else
    {
        std::cout << "read error" << std::endl;
    }

}

void testDo()
{
    //step1：实例化
    std::shared_ptr<PfNi::DigitalOut> obj = std::make_shared<PfNi::DigitalOut>();

    //step2：打开板卡

    std::string cardName = "PXI6528";   //板卡名称

    std::vector<int> ports; //板卡输入对应port号(3~5 共24路)
    ports.push_back(3);
    ports.push_back(4);
    ports.push_back(5);

    obj->initCard(cardName, ports);

    //step3：设置状态

    //step3-1：设置高状态(第0个port的第1路)
    obj->setValue(0, 0, true);

    //step3-2：设置低状态(第2个port的第3路)
    obj->setValue(1, 2, false);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {
        testDi();

        testDo();
    }
    catch(std::runtime_error err)
    {
        std::cout << err.what() << std::endl;
    }

    return a.exec();
}
