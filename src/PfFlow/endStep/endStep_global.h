#ifndef ENDSTEP_GLOBAL_H
#define ENDSTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ENDSTEP_LIBRARY)
#  define ENDSTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ENDSTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ENDSTEP_GLOBAL_H
    
    