#ifndef IFSTEP_GLOBAL_H
#define IFSTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IFSTEP_LIBRARY)
#  define IFSTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IFSTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // IFSTEP_GLOBAL_H
