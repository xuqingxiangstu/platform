#ifndef CREATESUBFLOW_H
#define CREATESUBFLOW_H

#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../src/PfCommon/jsoncpp/json.h"

#include "../property/nodeproperty.h"

/****************************宏定义******************************/

#define CONFIG_ELEMENT      "config"

//等效
#define EQ_ELEMENT              "equivalent"
#define EQ_SYSTEM_UUID_ELEMENT  "system_uuid"
#define EQ_SRC_SYS_TYPE_ELEMENT "head_src_sys_type"
#define EQ_SRC_SYS_CODE_ELEMENT "head_src_sys_code"
#define EQ_SRC_NODE_CODE_ELEMENT "head_src_node_code"

//流程

#define FLOW_ELEMENT            "flow"
#define FLOW_ATTR_UUID          "uuid"
#define FLOW_ATTR_DESCRIBE      "describe"

//子流程
#define SUB_FLOW_ELEMENT            "subFlow"
#define SUB_FLOW_ATTR_UUID          "uuid"
#define SUB_FLOW_ATTR_DESCRIBE      "describe"

//测试项
#define TEST_ITEM_ELEMENT            "testItem"
#define TEST_ITEM_ATTR_UUID          "uuid"
#define TEST_ITEM_ATTR_DESCRIBE      "describe"

#define JSON_ELEMENT        "json"
#define SUB_JSON_ELEMENT    "subJson"

#define TIMING_ELEMENT          "timing"

#define STOP_CONDITION_ELEMENT "stopCondition"
#define START_CONDITION_ELEMENT "startCondition"
#define SYSTEM_UUID_ELEMENT     "system_uuid"
#define DEST_SYSTEM_ELEMENT     "dest_system_uuid"
#define TABLE_ELEMENT           "table"
#define CODING_ELEMENT          "coding"
#define ACTION_ELEMENT          "action"
#define SLEEP_ELEMENT           "sleep"
#define FRAME_ELEMENT           "frame"
#define HEAD_ELEMENT            "head"
#define DATAFIELDS_ELEMENT      "datafields"
#define PARAM_ELEMENT           "param"
#define VALUE_ELEMENT           "value"
#define INIT_ELEMENT            "init"
#define CMD_ELEMENT             "cmd"
#define MIN_ELEMENT             "min"
#define MAX_ELEMENT             "max"
#define LINE_ELEMENT            "line"
#define RAND_ELEMENT            "random"
#define STEP_ELEMENT            "changeStep"


class createEquivalent
{
public:
    static TiXmlElement *element(nodeProperty *node);
private:
    static TiXmlElement *createTextElement(const std::string &eleName, const std::string &text);
};


class createSubFlow
{
public:  
    static TiXmlElement *element(nodeProperty *node, std::vector<nodeProperty*> subNode = {});
private:
    static TiXmlElement *createTextElement(const std::string &eleName, const std::string &text);
    static TiXmlElement *createDataFieldsElement(nodeProperty *node, std::vector<nodeProperty *>subNode);
    static TiXmlElement *createHeadElement(nodeProperty *node, std::vector<nodeProperty *>subNode);
};

class createFlow
{
public:
    static TiXmlElement *element(nodeProperty *node);
};

class createRoot
{
public:
    static TiXmlElement *element();
};

#endif // CREATESUBFLOW_H
