#ifndef ANALOGINADAPTER_GLOBAL_H
#define ANALOGINADAPTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ANALOGINADAPTER_LIBRARY)
#  define ANALOGINADAPTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ANALOGINADAPTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ANALOGINADAPTER_GLOBAL_H
    
    