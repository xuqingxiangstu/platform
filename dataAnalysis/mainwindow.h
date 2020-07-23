#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "projectnavigation.h"
#include "workspacearea.h"
#include "messagearea.h"
#include "versionform.h"
#include "./propertyui/propertywidget.h"

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
    void test();

    /**
     * @brief loadProject   加载已有工程
     * @param path
     */
    void loadProject(const QString &path);

    void closeEvent( QCloseEvent * event);
public slots:
    void onProjectAlreadySave(QString uuid);
signals:

    /**
     * @brief saveProject   保存某个工程
     */
    void saveProject(QString uuid);
private:
    projectNavigation *mProjectNavigationWidget;    ///< 导航区
    propertyWidget *mNavigationPropertyObj; //导航属性
    workspaceArea *mWorkSpaceAreaWidget;            ///< 工作区
    messageArea *mMessageAreaWidget;                ///< 消息区
    QStringList mSaveProjectUuid;
    bool isExit;
    versionForm *mVersionWidget;  ///< 版本号
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
