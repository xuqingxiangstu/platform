#ifndef BROADCASTADAPTER_GLOBAL_H
#define BROADCASTADAPTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BROADCASTADAPTER_LIBRARY)
#  define BROADCASTADAPTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BROADCASTADAPTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BROADCASTADAPTER_GLOBAL_H
    
    