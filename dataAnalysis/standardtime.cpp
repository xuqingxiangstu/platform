#include "standardtime.h"

QString standardTime::mDateTimeFormat = "yyyy/M/d H:mm:ss.z";

QDateTime standardTime::fromString(const QString &time)
{
    return QDateTime::fromString(time, mDateTimeFormat);
}


QString standardTime::toString(const QDateTime &time)
{
    return time.toString(mDateTimeFormat);
}
