#ifndef PRINTFSTEP_GLOBAL_H
#define PRINTFSTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PRINTFSTEP_LIBRARY)
#  define PRINTFSTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PRINTFSTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PRINTFSTEP_GLOBAL_H
    
    