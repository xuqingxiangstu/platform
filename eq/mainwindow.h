#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QLabel>
#include <QKeyEvent>
#include "templatetree.h"
#include "flowtree.h"
#include "recordnavigation.h"
#include "./propertyui/propertywidget.h"
#include "searchform.h"
#include "cmddecode.h"
#include "versionform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void onSave();

    void closeEvent( QCloseEvent * event);
public slots:
    /**
     * @brief onFlowChange  流程变化
     * @param sysName       系统名称
     * @param sysType       系统类型
     * @param testName      测试项
     * @param isNew         是否新创建
     * @param uuid
     */
    void onFlowChange(QString sysName, int sysType, QString testName, QString uuid, bool isNew);

    void onDeleteFlow(QString uuid);

    void onProjectAlreadySave(QString uuid);

    void onShowMessage(QString msg);

    void onTestMsg(Json::Value msg);

    void onSearch(Json::Value condition);
signals:
    void valueChange(QString uuid, QString attr, Json::Value val);
    void showCurItemProperty(QString uuid);
    void toShowProperty(QString, Json::Value);

    void setGroupPropertyEnable(QString propertyName, bool isEnable);

    /**
     * @brief saveProject   保存某个工程
     * @param item  记录item
     */
    void saveProject(QTreeWidgetItem *item);

    void updateProject(QString uuid);

    void updateProperty(QString propertyName, Json::Value value);

    void toSearch(QString uuid, Json::Value condition);
    void searchResult(Json::Value result);
private:
    /**
     * @brief initNavigationProperty    初始化导航属性
     */
    void initNavigationProperty();
private:
    templateTree *mTemplateTreeObj;
    recordNavigation *mRecordNavigationObj;
    propertyWidget *mNavigationPropertyObj; //导航属性
    propertyWidget *mPropertyWidgetObj; //流程属性
    QMap<QString, QWidget*> mFlowWidgetManager; //流程管理
    QString mCurFlowWidgetUuid; //当前流程UUID
    searchForm *mSearchForm;    ///< 搜索窗体
    bool isExit;
    QStringList mSaveProjectUuid;
    std::shared_ptr<cmdDecode> mCmdDecodeObj;   //协议解析句柄
    versionForm *mVersionWidget;  ///< 版本号
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
