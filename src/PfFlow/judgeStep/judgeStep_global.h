#ifndef JUDGESTEP_GLOBAL_H
#define JUDGESTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(JUDGESTEP_LIBRARY)
#  define JUDGESTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define JUDGESTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // JUDGESTEP_GLOBAL_H
    
    