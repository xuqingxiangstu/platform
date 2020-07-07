#include "argumentparse.h"

using namespace Pf;

argumentParse::argumentParse(QObject *parent) : QObject(parent)
{
    mIcdFrameAdpter = std::make_shared<PfIcdWorkBench::icdFrameAdapter>();
    mIcdFrameAdpter->init("./cfgfile/icd.xml");
}

void argumentParse::init()
{

}
