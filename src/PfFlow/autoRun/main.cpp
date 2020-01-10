#include <QCoreApplication>
#include <iostream>
#include "flow.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    flow fl;

    try
    {
        fl.init("./flow.xml");
        fl.runAllFlow();
    }
    catch(std::runtime_error err)
    {
        std::cout << err.what() << std::endl;
    }



    std::cout << "init ok" << std::endl;

    return a.exec();
}
