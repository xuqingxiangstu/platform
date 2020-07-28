#ifndef MAPPING_H
#define MAPPING_H

#include <QString>


/**
 * @brief The mapping class 关系映射
 */

class mapping
{
public:
    /**
     * @brief frameType 帧类型映射
     * @param type      类型
     * @return
     */
    static QString frameType(int type);
};

#endif // MAPPING_H
