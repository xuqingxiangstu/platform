#ifndef DRAGROLE_H
#define DRAGROLE_H

#include <string>
#include <memory>
#include "../src/PfCommon/jsoncpp/json.h"
#include <QMetaType>
#include "../property/nodeproperty.h"

class dragRole
{
public:
    enum nodeType
    {
        Node_Root = 0,              //跟节点
        Node_Cmd_Or_Param_Group,    //组节点
        Node_Cmd_Group,             //指令组

        Node_Flow,                  //流程
        Node_SubFlow,               //子流程
        Node_Cmd,                   //指令
        Node_Param_Group,           //参数组
        Node_Param,                 //参数

        Node_Size
    };

    /** json 属性 key **/
    const std::string mNodeKey[Node_Size] = {
        "",
        "",
        "",
        "flow",
        "sub_flow",
        "cmd",
        "param_group",
        "param"
    };

public:
    dragRole();

#if 1
    dragRole(const dragRole &other) = delete;
    dragRole &operator=(const dragRole &other) = delete;
    dragRole(const dragRole &&other) = delete;
    dragRole &operator =(dragRole &&other) = delete;
#else
    dragRole(const dragRole &other);
    dragRole &operator=(const dragRole &other);
    dragRole(const dragRole &&other);
#endif

    std::shared_ptr<dragRole> clone();

    void setProperty(nodeProperty *pro){mNodeProperty = std::shared_ptr<nodeProperty>(pro);}
    nodeProperty *getProperty(){return mNodeProperty.get();}

    void setNodeType(nodeType type){mCurCmdType = type;}
    nodeType getNodeType(){return mCurCmdType;}

    void setTableAndCoding(std::string table, int coding);

    //int tableNum(){return mCurTable;}
    //int codingNum(){return mCurCoding;}
    void setUuid(std::string uuid){mCurUuid = uuid;}
    std::string uuid(){return mCurUuid;}

    void setItemUuid(std::string uuid){mItemUuid = uuid;}
    std::string itemUuid(){return mItemUuid;}
private:
    nodeType mCurCmdType;
    std::string mCurTable;
    int mCurCoding;
    std::string mCurUuid;
    std::string mItemUuid;
    std::shared_ptr<nodeProperty> mNodeProperty;
};

//Q_DECLARE_METATYPE(dragRole)
Q_DECLARE_METATYPE(std::shared_ptr<dragRole>)

//Q_DECLARE_METATYPE(std::vector<std::shared_ptr<dragRole>>)

#endif // DRAGROLE_H
