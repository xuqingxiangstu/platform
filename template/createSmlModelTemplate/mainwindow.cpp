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
        QMessageBox::information(this, "提示", "请输入仿真模型名称");
        return ;
    }

    QString stepName = ui->lineEdit->text();

    QString root = QDir::currentPath();
    QString rootPath = root.left(root.lastIndexOf('/')) + "/src/PfSmlModel";
    QString stepFloader = rootPath + "/" + stepName;
    QDir dir(stepFloader);
    if(dir.exists())
    {
        QMessageBox::information(this, "提示", stepFloader + " 仿真模型已存在");
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
#include \"../algorithm/Algorithm.h\"\n\
\n\
#include <memory>\n\
\n\
/** 版本号 **/\n\
#define VERSION \"1.0.0\"\n\
\n\
\n\
class " + tmp + "SHARED_EXPORT " + className + " : public Algorithm\n\
{\n\
public:\n\
  " + className + "();\n\
  ~" + className + "();\n\
public:\n\
  void init(const TiXmlElement *ele) override;\n\
  void init(std::initializer_list<double> params) override;\n\
  int setAttribute(const std::string &attr, const void *value) override;\n\
  std::string version() override {return VERSION;}\n\
  std::string getClassName() override;\n\
  valueType value();\n\
private:\n\
  void calculate();\n\
  valueType mCurValue;   ///< 当前数值\n\
  std::mutex mMutex;   ///< 消息锁\n\
};\n\
\n\
      extern \"C\"\n\
      {\n\
          /**\n\
          * @brief 加载类\n\
          * @return 类指针\n\
          */\n\
          " + tmp + "SHARED_EXPORT Algorithm *LoadClass();\n\
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
      " + className + "::" + className + "()\n\
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
      void " + className + "::init(std::initializer_list<double> params)\n\
      {\n\
\n\
      }\n\
\n\
      int " + className + "::setAttribute(const std::string &attr, const void *value)\n\
      {\n\
          return 0;\n\
      }\n\
\n\
      std::string " + className + "::getClassName()\n\
      {\n\
          return VAR_NAME(" + className + ");\n\
      }\n\
\n\
      void " + className + "::calculate()\n\
      {\n\
\n\
      }\n\
\n\      valueType " + className + "::value()\n\
      {\n\
          return mCurValue;\n\
      }\n\
\n\
      extern \"C\"\n\
      {\n\
          Algorithm *LoadClass()\n\
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
win32:CONFIG(release, debug|release): TARGET = ../../../../algorithmLib/" + className + "\n\
else:win32:CONFIG(debug, debug|release): TARGET = ../../../../algorithmLib/" + className + "\n\
else:unix:!macx: TARGET = ../../../algorithmLib/" + className + "\n\
TEMPLATE = lib\n\
\n\
DEFINES += " + tmp + "_LIBRARY\n\
CONFIG += C++11\n\
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
else:unix:!macx: LIBS += -L$$OUT_PWD/../../../ -lTinyXml\n\
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
