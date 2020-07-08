#ifndef UT_ERROR_H
#define UT_ERROR_H

#include <exception>
#include <string>
#include <sstream>
#include <cstdio>
#include <stdexcept>
#include <iostream>

#include <mutex>
#include <QDebug>
#include <QString>
#include <QTextCodec>

#define INFO_STR(info) infoStr(info, __PRETTY_FUNCTION__, __FILE__, __LINE__)

inline std::string infoStr(std::string arg, char const * funcName, char const * fileName, int line)
{
    std::ostringstream strErr;
    strErr.str("");
    strErr << fileName << "(" << line << ")" << "-<" << funcName << ">: " << arg;
    return strErr.str();
}

#define THROW_EXCEPTION(err) throwSrcErr(err)

inline void throwSrcErr(std::string arg)
{
    throw std::runtime_error(arg);
}

#define UT_THROW_EXCEPTION(err) throwErr(err, __PRETTY_FUNCTION__, __FILE__, __LINE__)

inline void throwErr(std::string arg, char const * funcName, char const * fileName, int line)
{
    std::ostringstream strErr;
    strErr.str("");
    strErr << fileName << "(" << line << ")" << "-<" << funcName << ">: " << arg;
    throw std::runtime_error(strErr.str());
}

#define UT_SHOW(err) show(err, __PRETTY_FUNCTION__, __FILE__, __LINE__)

inline void show(std::string arg, char const * funcName, char const * fileName, int line)
{
    std::ostringstream strErr;

    strErr.str("");
    strErr << fileName << "(" << line << ")" << "-<" << funcName << ">: " << arg;

    std::cout << strErr.str() << std::endl;
}

#define SHOW(err) show(err)

inline void show(std::string arg)
{
#ifndef QT_NO_DEBUG

    QString utf8Str = QString::fromStdString(arg);
    arg = QTextCodec::codecForName("gb2312")->fromUnicode(utf8Str).data();

    std::cout << arg << std::endl;
#endif
}

#define QSHOW(err) qShow(err)

inline void qShow(std::string msg)
{
#ifndef QT_NO_DEBUG
    qDebug() << QString::fromStdString(msg);
#endif
}

#endif // UT_ERROR_H
