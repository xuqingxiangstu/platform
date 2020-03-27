#ifndef CREATESUBFLOW_H
#define CREATESUBFLOW_H

#include "../src/PfCommon/TinyXml/tinyxml.h"
#include "../src/PfCommon/jsoncpp/json.h"

#include "../property/nodeproperty.h"

/****************************宏定义******************************/
#define FLOW_ELEMENT        "subFlow"
#define FLOW_ATTR_UUID      "uuid"
#define FLOW_ATTR_DESCRIBE  "describe"

#define TIMING_ELEMENT          "timing"

#define STOP_CONDITION_ELEMENT "stopCondition"
#define START_CONDITION_ELEMENT "startCondition"
#define SYSTEM_UUID_ELEMENT     "system_uuid"
#define DEST_SYSTEM_ELEMENT     "dest_system"
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


class createSubFlow
{
public:
    createSubFlow();

public:
    void setProperty(nodeProperty *node, std::vector<nodeProperty*> subNode = {});
    TiXmlElement *toXml();
private:
    TiXmlElement *createTextElement(const std::string &eleName, const std::string &text);
    TiXmlElement *createDataFieldsElement(nodeProperty *node, std::vector<nodeProperty *>subNode);
    TiXmlElement *createHeadElement(nodeProperty *node, std::vector<nodeProperty *>subNode);
private:
    TiXmlElement *mSubFlowXmlEle;
};

#endif // CREATESUBFLOW_H
