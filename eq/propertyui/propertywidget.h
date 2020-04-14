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
    void showProperty(QString uuid, Json::Value value);

    /**
     * @brief updateProperty    更新属性值
     * @param propertyName      属性名称
     * @param value             值
     * @param isToUi            是否改变Ui
     */
    void onUpdateProperty(QString propertyName, Json::Value value);

    void onIntValueChanged(QtProperty *property, int val);
    void onDoubleValueChanged(QtProperty *property, double val);
    void onStringValueChanged(QtProperty *property, QString val);
    void onBoolValueChanged(QtProperty *property, bool val);

    void onEnumValueChanged(QtProperty *property, int val);

    void onPropertyChange(QtProperty *property);

    /**
     * @brief setGroupPropertyEnable    设置组是否使能
     * @param propertyName  属性组名称
     * @param isEnable      是否使能
     */
    void setGroupPropertyEnable(QString propertyName, bool isEnable);

    /**
     * @brief removeProperty    删除属性
     * @param propertyName      属性名称
     */
    void removeProperty(QString propertyName);

    /**
     * @brief addProperty   添加属性
     * @param fatherName    父名称
     * @param json          属性值
     */
    void addProperty(QString fatherName, Json::Value json);

    /**
     * @brief removeProperty    删除属性
     * @param propertyName      属性名称
     */
    void removeGroupProperty(QString propertyName);

    /**
     * @brief addProperty   添加属性
     * @param fatherName    父名称
     * @param json          属性值
     */
    void addGroupProperty(QString name);
signals:
    void valueChange(QString uuid, QString attr, Json::Value val);
private:
    QtProperty *createEnumProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);
    QtProperty *createIntProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue, const Json::Value &minValue, const Json::Value &maxValue);
    QtProperty *createStringProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);
    QtProperty *createBoolProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);
    QtProperty *createDoubleProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);

    QtProperty *createTriggerProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue);

    QtProperty *createProperty(const Json::Value &json);

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
    QString mCurUuid;
private:
    Ui::propertyWidget *ui;

};

Q_DECLARE_METATYPE(Json::Value)

#endif // PROPERTYWIDGET_H
