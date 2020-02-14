#include <QCoreApplication>

#include <iostream>
#include <vector>

#include <QDebug>

#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

using namespace Pf;

/**
 * @brief 帧解析与仿真例程
 * @example PfIcdDemo/main.cpp
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::shared_ptr<PfIcdWorkBench::icdFrameAdapter> frameAdapter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();

    try
    {
        //step2：初始化帧解析xml
        frameAdapter->init("./icd.xml");

        qDebug() << "init Ok\n";

        //step3：获取待解析的帧句柄
        auto obj = frameAdapter->getFrameObj("1");
        if(obj != nullptr)
        {
            std::vector<unsigned char> msg = {0xEB, 0x90, 0x0, 0x1D, 0xF0, 0xA1, 0x1,
                                              0x0, 0x0, 0x0, 0x1,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x0, 0x0, 0x0, 0x0,
                                              0x1,
                                              0x1
                                             };

            //step4：根据Excel中的数据进行仿真
            PfIcdWorkBench::byteArray sendMsg;

            obj->simulation(sendMsg, 0xF0A1);
            qDebug() << "仿真测试->";

            for(auto v : sendMsg)
            {
                std::cout << std::hex << (int)v << " ";
            }

            std::cout << std::endl;

            //step5：根据Excel中的数据进行解析
            std::vector<PfIcdWorkBench::icdOutConvertValueType> outV;
            qDebug() << "解析测试->\n";
            obj->parse(&sendMsg.at(0), sendMsg.size(), outV);

            //打印解析后的数据 ID + Value
            for(auto v : outV)
            {
                std::cout << std::get<0>(v) << " " << std::get<1>(v) << std::endl;
            }
        }
    }
    catch(std::runtime_error err)
    {
        //std::cout << err.what() << std::endl;
        qDebug() << err.what();
    }

    return a.exec();
}
