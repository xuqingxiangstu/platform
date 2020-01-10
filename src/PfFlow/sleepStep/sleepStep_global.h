#ifndef SLEEPSTEP_GLOBAL_H
#define SLEEPSTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SLEEPSTEP_LIBRARY)
#  define SLEEPSTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SLEEPSTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SLEEPSTEP_GLOBAL_H
    
    