#ifndef ANALOGOUTADAPTER_GLOBAL_H
#define ANALOGOUTADAPTER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ANALOGOUTADAPTER_LIBRARY)
#  define ANALOGOUTADAPTERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ANALOGOUTADAPTERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ANALOGOUTADAPTER_GLOBAL_H
    
    