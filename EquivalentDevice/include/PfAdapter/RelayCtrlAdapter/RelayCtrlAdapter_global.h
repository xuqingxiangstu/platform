#ifndef RELAYCTRLADAPTER_GLOBAL_H
#define RELAYCTRLADAPTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RELAYCTRLADAPTER_LIBRARY)
#  define RELAYCTRLADAPTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RELAYCTRLADAPTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RELAYCTRLADAPTER_GLOBAL_H
    
    