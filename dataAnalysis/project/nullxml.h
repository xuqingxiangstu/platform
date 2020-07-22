#ifndef NULLXML_H
#define NULLXML_H

#include <string>
#include <QString>

class nullXml
{
public:
    static void create(const std::string &filePath);
    static void create(const QString &filePath);
};

#endif // NULLXML_H
