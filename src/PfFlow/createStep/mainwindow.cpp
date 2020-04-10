#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRegExp rx("^[a-z]+$", Qt::CaseInsensitive);
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->lineEdit->setValidator(pReg);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::add);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add()
{
    if(ui->lineEdit->text().compare("") == 0)
    {
        QMessageBox::information(this, "提示", "请输流程部名称");
        return ;
    }

    QString stepName = ui->lineEdit->text();

    QString root = QDir::currentPath();
    QString rootPath = root.left(root.lastIndexOf('/')) + "/src/PfFlow";
    QString stepFloader = rootPath + "/" + stepName;
    QDir dir(stepFloader);
    if(dir.exists())
    {
        QMessageBox::information(this, "提示", stepFloader + " 流程部已存在");
        return ;
    }
    std::string className = stepName.toStdString();
    dir.mkdir(stepFloader);

    {
        std::ofstream out;
        std::string filePath;

        filePath = stepFloader.toStdString() + "/" + className + ".h";
        out.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

        std::string tmp = className;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int))std::toupper);

        std::ostringstream strBuf;
        strBuf << "\
#ifndef " + tmp + "_H\n\
#define " + tmp + "_H\n\
\n\
#include \"" + className + "_global.h\"\n\
\n\
#include \"../runObj/runobj.h\"\n\
\n\
class " + tmp + "SHARED_EXPORT " + className + " : public runObj\n\
{\n\
public:\n\
    " + className + "();\n\
    ~" + className + "();\n\
public:\n\
    void init(const TiXmlElement *ele) override;\n\
    std::string getClassName() override;\n\
    bool exec() override;\n\
    std::string getVarValue(std::string) override;\n\
    bool getResult() override;\n\
    std::string outText() override;\n\
    void setAutoRunObj(runObj *obj) override;\n\
    void setAdapter(Pf::PfAdapter::PfAdapterManager *adapter) override;\n\
private:\n\
    runObj *mRunObj;    ///< 运行类\n\
    bool mResult; ///< 测试结果\n\
    Pf::PfAdapter::PfAdapterManager *mAdapter; ///< 适配器管理\n\
    std::string strOutText; ///< 输出信息\n\
};\n\
\n\
extern \"C\"\n\
{\n\
    /**\n\
    * @brief 加载类\n\
    * @return 类指针\n\
    */\n\
    " + tmp + "SHARED_EXPORT runObj *LoadClass();\n\
}\n\
\n\
#endif // " + tmp + "_H\n\
    \n\
    ";
        out << strBuf.str();
        out.close();
    }

    {
        std::ofstream out;
        std::string filePath;

        filePath = stepFloader.toStdString() + "/" + className + ".cpp";
        out.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

        std::string tmp = className;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int))std::toupper);

        std::ostringstream strBuf;
        strBuf << "\
#include \"" + className + ".h\"\n\
\n\
#include \"../../PfCommon/tools/ut_error.h\"\n\
\n\
" + className + "::" + className + "():\n\
    mRunObj(nullptr),\n\
    mAdapter(nullptr),\n\
    strOutText(""),\n\
    mResult(false)\n\
{\n\
}\n\
\n\
" + className + "::~" + className + "()\n\
{\n\
\n\
}\n\
\n\
void " + className + "::init(const TiXmlElement *ele)\n\
{\n\
\n\
}\n\
\n\
std::string " + className + "::getClassName()\n\
{\n\
    return VAR_NAME(" + className + ");\n\
}\n\
\n\
bool " + className + "::exec()\n\
{\n\
#ifdef DEBUG_OBJ\n\
    UT_SHOW(\"[" + className + "]\");\n\
#endif\n\
\n\
    return mResult;\n\
}\n\
std::string " + className + "::getVarValue(std::string)\n\
{\n\
    return \"\";\n\
}\n\
\n\
bool " + className + "::getResult()\n\
{\n\
    return mResult;\n\
}\n\
\n\
std::string " + className + "::outText()\n\
{\n\
    return strOutText;\n\
}\n\
\n\
void " + className + "::setAutoRunObj(runObj *obj)\n\
{\n\
    mRunObj = obj;\n\
}\n\
\n\
void " + className + "::setAdapter(Pf::PfAdapter::PfAdapterManager *adapter)\n\
{\n\
    mAdapter = adapter;\n\
}\n\
\n\
extern \"C\"\n\
{\n\
    runObj *LoadClass()\n\
    {\n\
        return new " + className + "();\n\
    }\n\
}\n\
\n\
    \n\
    ";
        out << strBuf.str();
        out.close();
    }

    {
        std::ofstream out;
        std::string filePath;

        filePath = stepFloader.toStdString() + "/" + className + ".pro";
        out.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

        std::string tmp = className;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int))std::toupper);

        std::ostringstream strBuf;
        strBuf << "\
QT       -= gui\n\
\n\
TARGET = ../../../../stepLib/" + className + "\n\
TEMPLATE = lib\n\
CONFIG += C++11\n\
DEFINES += " + tmp + "_LIBRARY\n\
\n\
SOURCES += " + className + ".cpp\n\
\n\
HEADERS += " + className + ".h\\\n\
    " + className + "_global.h\n\
\n\
unix {\n\
target.path = /usr/lib\n\
INSTALLS += target\n\
}\n\
\n\
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml\n\
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../ -lTinyXml\n\
\n\
\n\
    \n\
    ";
        out << strBuf.str();
        out.close();
    }

    {
        std::ofstream out;
        std::string filePath;

        filePath = stepFloader.toStdString() + "/" + className + "_global.h";
        out.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

        std::string tmp = className;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int))std::toupper);

        std::ostringstream strBuf;
        strBuf << "\
#ifndef " + tmp + "_GLOBAL_H\n\
#define " + tmp + "_GLOBAL_H\n\
\n\
#include <QtCore/qglobal.h>\n\
\n\
#if defined(" + tmp + "_LIBRARY)\n\
#  define " + tmp + "SHARED_EXPORT Q_DECL_EXPORT\n\
#else\n\
#  define " + tmp + "SHARED_EXPORT Q_DECL_IMPORT\n\
#endif\n\
\n\
#endif // " + tmp + "_GLOBAL_H\n\
    \n\
    ";
        out << strBuf.str();
        out.close();

              QMessageBox::information(this, "提示", "创建成功");
    }
}
