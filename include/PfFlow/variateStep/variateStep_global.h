#ifndef VARIATESTEP_GLOBAL_H
#define VARIATESTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VARIATESTEP_LIBRARY)
#  define VARIATESTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VARIATESTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VARIATESTEP_GLOBAL_H
    
    