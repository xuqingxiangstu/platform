#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QWidget>

#include "../src/PfCommon/jsoncpp/json.h"

#include "QtPropertyBrowser/include/qtpropertymanager.h"
#include "QtPropertyBrowser/include/qteditorfactory.h"
#include "QtPropertyBrowser/include/qttreepropertybrowser.h"
#include "QtPropertyBrowser/include/qtbuttonpropertybrowser"
#include "QtPropertyBrowser/include/qtgroupboxpropertybrowser.h"
#include "QtPropertyBrowser/include/QtCheckBoxFactory"

namespace Ui {
class propertyWidget;
}

/************属性类型宏定义***************/
#define PROPERTY_STRING "string"
#define PROPERTY_INT    "int"
#define PROPERTY_ENUM   "enum"
#define PROPERTY_DOUBLE "double"
#define PROPERTY_BOOL   "bool"
#define PROPERTY_TRIGGER    "trigger"

/***********trigger*********************/

#define TRIGGER_NAME    "name"
#define TRIGGER_TABLE   "table"
#define TRIGGER_CODING  "coding"

#define TRIGGER_NO      "无"

//#define DEBUG_PRINT 0

class propertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit propertyWidget(QWidget *parent = 0);
    ~propertyWidget();
public slots:
    void showProperty(Json::Value value);

    void onIntValueChanged(QtProperty *property, int val);
    void onDoubleValueChanged(QtProperty *property, double val);
    void onStringValueChanged(QtProperty *property, QString val);
    void onBoolValueChanged(QtProperty *property, bool val);

    void onEnumValueChanged(QtProperty *property, int val);

    void onPropertyChange(QtProperty *property);
signals:
    void valueChange(QString attr, Json::Value val);
private:
    QtProperty *createEnumProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);
    QtProperty *createIntProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);
    QtProperty *createStringProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);
    QtProperty *createBoolProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);
    QtProperty *createDoubleProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);

    QtProperty *createTriggerProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);

    void deleteAllExistProperty();

    void updateSimChange();

    void setChange(std::string type);
private:
    QtAbstractPropertyBrowser *mEditorProperty; //属性树
    QtGroupPropertyManager *mGroupManager; //组管理
    QtEnumPropertyManager  *mEnumManager; //枚举管理
    QtIntPropertyManager    *mIntManager; //整型管理
    QtStringPropertyManager *mstrManager; //字符管理
    QtBoolPropertyManager   *mboolManager;//布尔管理
    QtDoublePropertyManager *mDoubleManager;//浮点管理

    bool isUpDate;
private:
    Ui::propertyWidget *ui;

};

Q_DECLARE_METATYPE(Json::Value)

#endif // PROPERTYWIDGET_H
