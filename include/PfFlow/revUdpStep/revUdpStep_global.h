#ifndef REVUDPSTEP_GLOBAL_H
#define REVUDPSTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(REVUDPSTEP_LIBRARY)
#  define REVUDPSTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define REVUDPSTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // REVUDPSTEP_GLOBAL_H
    
    