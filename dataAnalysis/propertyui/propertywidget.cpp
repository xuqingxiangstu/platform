#include "propertywidget.h"
#include "ui_propertywidget.h"
#include "standardtime.h"
#include "../property/nodeproperty.h"

#include <QDateTime>

#include <QDebug>

propertyWidget::propertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::propertyWidget)
{
    ui->setupUi(this);

    //属性管理
    mGroupManager =new QtGroupPropertyManager(this);
    mEnumManager  =new QtEnumPropertyManager(this);
    mIntManager   =new QtIntPropertyManager(this);
    mDoubleManager = new QtDoublePropertyManager(this);
    mstrManager   =new QtStringPropertyManager(this);
    mboolManager = new QtBoolPropertyManager(this);
    mDateTimeManager = new QtDateTimePropertyManager(this);
    mFlagManager = new QtFlagPropertyManager(this);

    mEditorProperty = new QtTreePropertyBrowser(this);

    //为每个属性管理设置Factory
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
    mEditorProperty->setFactoryForManager(mIntManager, spinBoxFactory);
    QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
    mEditorProperty->setFactoryForManager(mEnumManager, comboBoxFactory);
    QtLineEditFactory   *lineEditFactory = new QtLineEditFactory(this);
    mEditorProperty->setFactoryForManager(mstrManager,lineEditFactory);
    QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
    mEditorProperty->setFactoryForManager(mDoubleManager,doubleSpinBoxFactory);
    QtDateTimeEditFactory *dateTimeEditFactor = new QtDateTimeEditFactory(this);
    mEditorProperty->setFactoryForManager(mDateTimeManager, dateTimeEditFactor);

    QtCheckBoxFactory *flagEditFactor = new QtCheckBoxFactory(this);
    mEditorProperty->setFactoryForManager(mFlagManager->subBoolPropertyManager(), flagEditFactor);
    //connect(mstrManager, &QtStringPropertyManager::propertyChanged, this, &propertyWidget::onPropertyChange);

    connect(mIntManager, &QtIntPropertyManager::valueChanged, this, &propertyWidget::onIntValueChanged);
    connect(mDoubleManager, &QtDoublePropertyManager::valueChanged, this, &propertyWidget::onDoubleValueChanged);
    connect(mstrManager, &QtStringPropertyManager::valueChanged, this, &propertyWidget::onStringValueChanged);
    connect(mboolManager, &QtBoolPropertyManager::valueChanged, this, &propertyWidget::onBoolValueChanged);

    connect(mEnumManager, &QtEnumPropertyManager::valueChanged, this, &propertyWidget::onEnumValueChanged);
    connect(mDateTimeManager, &QtDateTimePropertyManager::valueChanged, this, &propertyWidget::onDateTimeValueChanged);

    connect(mFlagManager, &QtFlagPropertyManager::valueChanged, this, &propertyWidget::onFlagValueChanged);

    ui->horizontalLayout->addWidget(mEditorProperty);
}

propertyWidget::~propertyWidget()
{
    delete ui;
}
#if 0
void propertyWidget::showProperty(QString json)
{

}
#endif
void propertyWidget::showProperty(QString uuid, Json::Value value)
{
    isUpDate = false;

    mCurUuid = uuid;

    //显示之前先删除之前属性
    deleteAllExistProperty();
#ifdef DEBUG_PRINT
    qDebug() << value.toStyledString().c_str();
#endif    

    if(value.empty())
        return ;

    Json::Value propertyJs = value["property"];
    if(propertyJs.isNull())
        return ;
    for(int index = 0; index < propertyJs.size(); index++)
    {
        Json::Value groupJs = propertyJs[index];
        std::string groupName = groupJs["group"].asString();
        bool isEnable = groupJs["readOnly"].asBool();
        bool isShow = groupJs["isVisible"].asBool();

        if(!isShow)
            continue;

        QtProperty *groupItem = mGroupManager->addProperty(groupName.c_str());

        groupItem->setEnabled(!isEnable);

        Json::Value nodeJs = groupJs["node"];

        for(int nodeIndex = 0; nodeIndex < nodeJs.size(); nodeIndex++)
        {
            QtProperty *item = createProperty(nodeJs[nodeIndex]);

            if(item)
                groupItem->addSubProperty(item);
        }

        mEditorProperty->addProperty(groupItem);
    }

    //更新仿真变化
    updateSimChange();

    isUpDate = true;
}

QtProperty *propertyWidget::createProperty(const Json::Value &nodeJs)
{
    QtProperty *item = nullptr;

    bool isShow = nodeJs["isVisible"].asBool();
    if(!isShow)
        return item;

    std::string attr = nodeJs["attr"].asString();
    std::string type = nodeJs["type"].asString();
    bool isReadOnly = nodeJs["readOnly"].asBool();
    Json::Value initValue = nodeJs["initValue"];
    Json::Value curValue = nodeJs["curValue"];

    if(PROPERTY_INT == type)
    {
        item = createIntProperty(isReadOnly, attr, initValue, curValue, nodeJs["minValue"], nodeJs["maxValue"]);
    }
    else if(PROPERTY_DOUBLE == type)
    {
        item = createDoubleProperty(isReadOnly, attr, initValue, curValue);
    }
    else if(PROPERTY_STRING == type)
    {
        item = createStringProperty(isReadOnly, attr, initValue, curValue);
    }
    else if(PROPERTY_ENUM == type)
    {
        item = createEnumProperty(isReadOnly, attr, initValue, curValue);
    }
    else if(PROPERTY_TRIGGER == type)
    {
        item = createTriggerProperty(isReadOnly, attr, initValue, curValue);
    }
    else if(PROPERTY_TIME == type)
    {
        item = createDateTimeProperty(isReadOnly, attr, initValue, curValue);
    }
    else if(PROPERTY_FLAGS == type)
    {
        item = createFlagProperty(isReadOnly, attr, initValue, curValue);
    }
    return item;
}

QtProperty *propertyWidget::createTriggerProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue)
{
    QtProperty *property = mEnumManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    int curIndex = 0;

    //modify xqx 2020-6-7 12:32:50 当前值为空时，显示第一个空

    //if(!curValue.isObject() || !initValue.isArray())
    if(!initValue.isArray())
        return property;

    QStringList listName;

    QMap<int, QVariant> enumDatas;

    for(int index = 0; index < initValue.size(); index++)
    {
        std::string tmp = initValue[index][TRIGGER_NAME].asString();

        if(!curValue.isNull())
        {
            std::string tmp1 = initValue[index].toStyledString();
            std::string tmp2 = curValue.toStyledString();
            //modify xqx 2020-6-7 13:22:21 判断字符串相等，启动条件时异常
            //if(initValue[index] == curValue)
            if(tmp1 == tmp2)
            {
                curIndex = index;
            }
        }
        listName.append(tmp.c_str());

        QVariant var;
        var.setValue(initValue[index]);

        enumDatas[index] = var;
    }

    mEnumManager->setEnumNames(property, listName);
    mEnumManager->setData(property, enumDatas);
    mEnumManager->setValue(property, curIndex);

    return property;
}

QtProperty *propertyWidget::createEnumProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue)
{
    QtProperty *property = mEnumManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    if(!curValue.isString() || !initValue.isArray())
        return property;

    int curIndex = 0;
    std::string v;
    if(!curValue.isNull())
        v = curValue.asString();

    QStringList listName;
    QMap<int, QVariant> enumDatas;

    for(int index = 0; index < initValue.size(); index++)
    {
        std::string tmp = initValue[index].asString();

        if(tmp == v)
            curIndex = index;

        QVariant var;
        var.setValue(initValue[index]);

        enumDatas[index] = var;

        listName.append(tmp.c_str());
    }

    mEnumManager->setEnumNames(property, listName);
    mEnumManager->setData(property, enumDatas);
    mEnumManager->setValue(property, curIndex);

    return property;
}

QtProperty *propertyWidget::createFlagProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue)
{

    QtProperty *property = mFlagManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    if(!initValue.isArray())
        return property;

    QStringList listName;
    int flagVal = 0;
    Json::Value tmpJs;

    if(!curValue.isNull() && curValue.isArray())
    {
        tmpJs = curValue;
    }
    else
    {
        tmpJs = initValue;
    }

    for(int index = 0; index < tmpJs.size(); index++)
    {
        Json::Value obj = tmpJs[index];

        std::string tmp = obj["name"].asString();

        bool isEn = obj["enable"].asBool();
        if(isEn)
        {
            flagVal |= (1 << index);
        }

        listName.append(tmp.c_str());
    }

    mFlagManager->setFlagNames(property, listName);
    mFlagManager->setValue(property, flagVal);

    return property;
}

QtProperty *propertyWidget::createIntProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue, const Json::Value &minValue, const Json::Value &maxValue)
{
    QtProperty *property = mIntManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    if(!curValue.isInt() || !initValue.isInt())
        return property;

    if(!curValue.isNull())
        mIntManager->setValue(property, curValue.asInt());
    else
        mIntManager->setValue(property, initValue.asInt());

    if(!minValue.isNull() && minValue.isInt())
        mIntManager->setMinimum(property, minValue.asInt());

    if(!maxValue.isNull() && maxValue.isInt())
        mIntManager->setMaximum(property, maxValue.asInt());

    return property;
}

QtProperty *propertyWidget::createStringProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue)
{
    QtProperty *property = mstrManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    if(!curValue.isString() || !initValue.isString())
        return property;

    if(!curValue.isNull())
        mstrManager->setValue(property, curValue.asString().c_str());
    else
        mstrManager->setValue(property, initValue.asString().c_str());

    return property;
}

QtProperty *propertyWidget::createBoolProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue)
{
    QtProperty *property = mboolManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    if(!curValue.isBool() || !initValue.isBool())
        return property;

    if(!curValue.isNull())
        mboolManager->setValue(property, curValue.asBool());
    else
        mboolManager->setValue(property, initValue.asBool());

    return property;
}

QtProperty *propertyWidget::createDateTimeProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue)
{
    QtProperty *property = mDateTimeManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    if(!curValue.isString() || !initValue.isString())
        return property;

    QDateTime curTime;

    if(!curValue.isNull())
        curTime = standardTime::fromString(curValue.asString().c_str());
    else
        curTime = standardTime::fromString(initValue.asString().c_str());

    mDateTimeManager->setValue(property, curTime);

    return property;
}

QtProperty *propertyWidget::createDoubleProperty(const bool &isReadOnlay, const std::string &attrName, const Json::Value &initValue, const Json::Value &curValue)
{
    QtProperty *property = mDoubleManager->addProperty(attrName.c_str());

    property->setEnabled(!isReadOnlay);

    if(!curValue.isDouble() || !initValue.isDouble())
        return property;

    if(!curValue.isNull())
        mDoubleManager->setValue(property, curValue.asFloat());
    else
        mDoubleManager->setValue(property, initValue.asFloat());

    return property;
}

void propertyWidget::deleteAllExistProperty()
{
    mEditorProperty->clear();
    mEnumManager->clear();
    mGroupManager->clear();
    mIntManager->clear();
    mstrManager->clear();
    mboolManager->clear();
    mDoubleManager->clear();
    mDateTimeManager->clear();
    mFlagManager->clear();
}

void propertyWidget::onIntValueChanged(QtProperty *property, int val)
{
    if(isUpDate)
        emit valueChange(mCurUuid, property->propertyName(), Json::Value(val));
#ifdef DEBUG_PRINT
    qDebug() << property->propertyName();
    qDebug() << QString::number(val, 10);
#endif
}

void propertyWidget::onDoubleValueChanged(QtProperty *property, double val)
{
    if(isUpDate)
        emit valueChange(mCurUuid, property->propertyName(), Json::Value(val));
#ifdef DEBUG_PRINT
    qDebug() << property->propertyName();
    qDebug() << QString::number(val);
#endif
}

void propertyWidget::onStringValueChanged(QtProperty *property, QString val)
{
    if(isUpDate)
        emit valueChange(mCurUuid, property->propertyName(), Json::Value(val.toStdString()));
#ifdef DEBUG_PRINT
    qDebug() << property->propertyName();
    qDebug() << val;
#endif
}

void propertyWidget::onDateTimeValueChanged(QtProperty *property, const QDateTime &val)
{
    if(isUpDate)
        emit valueChange(mCurUuid, property->propertyName(), Json::Value(standardTime::toString(val).toStdString()));
}

void propertyWidget::onBoolValueChanged(QtProperty *property, bool val)
{
    if(isUpDate)
        emit valueChange(mCurUuid, property->propertyName(), Json::Value(val));

#ifdef DEBUG_PRINT
    qDebug() << property->propertyName();
    if(val)
        qDebug() <<"true";
    else
        qDebug() <<"false";
#endif
}

void propertyWidget::setGroupPropertyEnable(QString propertyName, bool isEnable)
{
    foreach(QtProperty *pro, mGroupManager->properties())
    {
        if( (pro->propertyName().compare(propertyName) == 0))
        {
            pro->setEnabled(!isEnable);
            break;
        }
    }    
}

void propertyWidget::addGroupProperty(QString name)
{
    bool notFind = true;

    foreach(QtProperty *pro, mEditorProperty->properties())
    {
        if( (pro->propertyName().compare(name) == 0))
        {
            notFind = false;
        }
    }

    if(notFind)
    {
        QtProperty *groupItem = mGroupManager->addProperty(name);

        mEditorProperty->addProperty(groupItem);
    }
}

void propertyWidget::addProperty(QString fatherName, Json::Value json)
{
    isUpDate = false;
    foreach(QtProperty *pro, mEditorProperty->properties())
    {
        if( (pro->propertyName().compare(fatherName) == 0))
        {
            if(!json.isNull())
            {
                QtProperty *item= createProperty(json);
                if(item != nullptr)
                    pro->addSubProperty(item);
            }

            break;
         }
    }
    isUpDate = true;
}

void propertyWidget::removeProperty(QString propertyName)
{
    foreach(QtProperty *groupPro, mGroupManager->properties())
    {
        foreach(QtProperty *pro, groupPro->subProperties())
        {
            if( (pro->propertyName().compare(propertyName) == 0))
            {
               groupPro->removeSubProperty(pro);
               return ;
            }
        }
    }
}

void propertyWidget::removeGroupProperty(QString propertyName)
{
    foreach(QtProperty *groupPro, mEditorProperty->properties())
    {
        if( (groupPro->propertyName().compare(propertyName) == 0))
        {
            mEditorProperty->removeProperty(groupPro);

            return ;
        }
    }
}

void propertyWidget::updateSimChange()
{

}

void propertyWidget::setChange(std::string type)
{

}

void propertyWidget::onFlagValueChanged(QtProperty *property, int val)
{
    if(isUpDate)
    {
        Json::Value array;

        QStringList names = mFlagManager->flagNames(property);

        for(int index = 0; index < names.size(); index++)
        {
            Json::Value tmpV;

            tmpV["name"] = names[index].toStdString();

            if( (val >> index) & 0x1)
            {
                tmpV["enable"] = true;
            }
            else
            {
                tmpV["enable"] = false;
            }

            array.append(tmpV);
        }

        emit valueChange(mCurUuid, property->propertyName(), array);
#if 0
        QString text = property->valueText();



        if(text.compare("") == 0)
        {
            array.append("");
        }
        else
        {
            QStringList res = text.split("|");
            foreach (QString v, res)
            {
                array.append(v.toStdString());
            }
        }


#endif
    }
}

void propertyWidget::onEnumValueChanged(QtProperty *property, int val)
{    
    if(isUpDate)
    {
        emit valueChange(mCurUuid, property->propertyName(), mEnumManager->data(property).value<Json::Value>());
    }
#ifdef DEBUG_PRINT
    qDebug() << property->propertyName();
    qDebug() << mEnumManager->enumNames(property).at(val);
#endif
}


void propertyWidget::onPropertyChange(QtProperty *property)
{
    if(isUpDate)
        emit valueChange(mCurUuid, property->propertyName(), Json::Value(mstrManager->value(property).toStdString()));
#ifdef DEBUG_PRINT
    qDebug() << property->propertyName();
    qDebug() << mstrManager->value(property);
#endif
}

void propertyWidget::onUpdateProperty(QString propertyName, Json::Value value)
{
    isUpDate = false;

    foreach(QtProperty *groupPro, mEditorProperty->properties())
    {
        foreach (QtProperty *pro, groupPro->subProperties())
        {
            if(pro->propertyName().compare(propertyName) == 0)
            {
                if(dynamic_cast<QtStringPropertyManager*>(pro->propertyManager()))
                {
                    if(value.isString())
                        dynamic_cast<QtStringPropertyManager*>(pro->propertyManager())->setValue(pro, value.asString().c_str());
                }
                else if(dynamic_cast<QtIntPropertyManager*>(pro->propertyManager()))
                {
                    if(value.isInt())
                        dynamic_cast<QtIntPropertyManager*>(pro->propertyManager())->setValue(pro, value.asInt());
                }
                else if(dynamic_cast<QtBoolPropertyManager*>(pro->propertyManager()))
                {
                    if(value.isBool())
                        dynamic_cast<QtBoolPropertyManager*>(pro->propertyManager())->setValue(pro, value.asBool());
                }
                break;
            }
        }
    }

    isUpDate = true;
}
