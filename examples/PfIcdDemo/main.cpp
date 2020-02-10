#include <QCoreApplication>

#include <iostream>

#include "../../src/PfIcdWorkBench/icdFrameAdapter/icdframeadapter.h"

using namespace Pf;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::shared_ptr<PfIcdWorkBench::icdFrameAdapter> frameAdapter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();

    try
    {
        frameAdapter->init("./icd.xml");
    }
    catch(std::runtime_error err)
    {
        std::cout << err.what() << std::endl;
    }

    return a.exec();
}
