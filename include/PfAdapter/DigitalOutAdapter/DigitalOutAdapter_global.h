#ifndef DIGITALOUTADAPTER_GLOBAL_H
#define DIGITALOUTADAPTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DIGITALOUTADAPTER_LIBRARY)
#  define DIGITALOUTADAPTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DIGITALOUTADAPTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DIGITALOUTADAPTER_GLOBAL_H
    
    