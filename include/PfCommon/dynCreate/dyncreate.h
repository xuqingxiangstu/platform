#ifndef DYNCREATE_H
#define DYNCREATE_H

/**
  * @file dynCreate.h
  * @breif  动态创建类
  * @author xqx
  * @version V1.0
  * @date 2019-12-31
  * @copyright (C)2019 北京交通大学智能测控技术中心
  */

#include "dyncreate_global.h"
#include <iostream>

namespace Pf
{
    /**
     * @brief 公共模块 包括 动态生成类、CRC算法等
     */
    namespace PfCommon
    {
        class DynObject;

        struct DYNCREATESHARED_EXPORT RuntimeClass
        {
            char *classname;
            DynObject* (*pCreateFn)();
            void* CreateObject();
            static RuntimeClass* LoadByName(std::string& classname);

            static RuntimeClass* pFirstClass;
            RuntimeClass* pBaseClass;
            RuntimeClass* pNextClass;
        };

        struct DYNCREATESHARED_EXPORT Class_List
        {
            Class_List(){};
            Class_List(RuntimeClass* pNewClass);
        };

        #define Runtime_Class(classname)	\
            (&classname::class##classname)

        #define Declare_RTTI(classname)	\
            public:	\
            static RuntimeClass class##classname;	\
            virtual RuntimeClass* GetRuntimeClass() const;	\

        #define Implement_RTTI(classname,basename)	\
            static char lpsz##classname[]=#classname;	\
            RuntimeClass classname::class##classname=	\
            {lpsz##classname,NULL,Runtime_Class(basename),NULL};	\
            static Class_List _init_##classname(&classname::class##classname);	\
            RuntimeClass* classname::GetRuntimeClass() const\
        {return &classname::class##classname;}	\

        #define Declare_DynCreate(classname)	\
            Declare_RTTI(classname)	\
            static DynObject* CreateObject();	\

        #define Implement_DynCreate(classname,basename);	\
            DynObject* classname::CreateObject()	\
            {return new classname;}	\
            static char lpsz##classname[]=#classname;	\
            RuntimeClass classname::class##classname=\
            {lpsz##classname,classname::CreateObject,Runtime_Class(basename),NULL};	\
            static Class_List _init_##classname(&classname::class##classname);	\
            RuntimeClass* classname::GetRuntimeClass() const\
            {return &classname::class##classname;}	\

        class DYNCREATESHARED_EXPORT DynObject
        {
        public:
            virtual ~DynObject(void){}
            virtual RuntimeClass* GetRuntimeClass() const
            {
                return &DynObject::classDynObject;
            }            
            bool IsKindOf(const RuntimeClass* pClass) const;
            static RuntimeClass classDynObject;
        };
    }
}
#endif // DYNCREATE_H
