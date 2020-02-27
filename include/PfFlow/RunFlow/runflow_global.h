#ifndef RUNFLOW_GLOBAL_H
#define RUNFLOW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RUNFLOW_LIBRARY)
#  define RUNFLOWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RUNFLOWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RUNFLOW_GLOBAL_H
