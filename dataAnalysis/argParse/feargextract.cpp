#include "feargextract.h"

#include <QDebug>

feArgExtract::feArgExtract(QObject *parent):
    argExtract(parent)
{

}

feArgExtract::~feArgExtract()
{

}

void feArgExtract::extract(const Json::Value &otherParam, std::shared_ptr<PfIcdWorkBench::frameObj> frameObj, const Json::Value &result)
{
    if(!result["region"].isNull())
    {
        Json::Value regionValue = result["region"];

        if(!regionValue["table_num"].isNull())
        {
            unsigned int table = regionValue["table_num"].asUInt();

            Json::Value array = regionValue["data"];
            for(int index = 0; index < array.size(); index++)
            {
                int coding = array[index]["coding"].asInt();


                qDebug() << "[FE]->" << QString::number(table, 16) << "," << QString::number(coding) << "," << array[index]["value"].asString().c_str();
            }
        }
    }
}
