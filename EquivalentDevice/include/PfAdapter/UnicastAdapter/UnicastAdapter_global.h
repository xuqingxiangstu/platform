#ifndef UNICASTADAPTER_GLOBAL_H
#define UNICASTADAPTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UNICASTADAPTER_LIBRARY)
#  define UNICASTADAPTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UNICASTADAPTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UNICASTADAPTER_GLOBAL_H
    
    