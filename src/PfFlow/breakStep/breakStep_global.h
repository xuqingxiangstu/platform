#ifndef BREAKSTEP_GLOBAL_H
#define BREAKSTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BREAKSTEP_LIBRARY)
#  define BREAKSTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BREAKSTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BREAKSTEP_GLOBAL_H
    
    