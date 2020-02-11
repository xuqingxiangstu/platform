#include <QCoreApplication>

#include <iostream>
#include <vector>

#include <QDebug>

#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

using namespace Pf;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::shared_ptr<PfIcdWorkBench::icdFrameAdapter> frameAdapter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();

    try
    {
        frameAdapter->init("./icd.xml");

        std::cout << "init Ok" << std::endl;

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

            std::vector<PfIcdWorkBench::icdOutConvertValueType> outV;

            obj->parse(&msg.at(0), msg.size(), outV);

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
