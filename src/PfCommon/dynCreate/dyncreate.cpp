#include "dyncreate.h"

#include <string.h>

namespace Pf
{
    namespace PfCommon
    {
        RuntimeClass* RuntimeClass::pFirstClass=NULL;
        static char lpszBaseClass[]="DynObject";
        RuntimeClass DynObject::classDynObject={lpszBaseClass,NULL,NULL,NULL};
        static Class_List _init_BaseClass(&DynObject::classDynObject);


        Class_List::Class_List(RuntimeClass* pNewClass)
        {
            pNewClass->pNextClass=RuntimeClass::pFirstClass;
            RuntimeClass::pFirstClass=pNewClass;
        }

        void* RuntimeClass::CreateObject()
        {
            if(pCreateFn == NULL)
            {
                return NULL;
            }
            void* pObject = NULL;
            pObject = (*pCreateFn)();
            return pObject;
        }

        RuntimeClass* RuntimeClass::LoadByName(std::string& classname)
        {
            RuntimeClass* pClass;
            for(pClass = pFirstClass; pClass != NULL; pClass=pClass->pNextClass)
            {
                if(strcmp(classname.c_str(),pClass->classname) == 0)
                {
                    return pClass;
                }
            }
            return NULL;
        }       

        bool DynObject::IsKindOf(const RuntimeClass* pClass) const
        {
            RuntimeClass* pClassThis = GetRuntimeClass();
            while(pClassThis != NULL)
            {
                if(strcmp(pClassThis->classname,pClass->classname) == 0)
                {
                    return true;
                }
                pClassThis = pClassThis->pBaseClass;
            }
            return false;
        }
    }
}
