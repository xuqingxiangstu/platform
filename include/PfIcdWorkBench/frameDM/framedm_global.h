#ifndef FRAMEDM_GLOBAL_H
#define FRAMEDM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEDM_LIBRARY)
#  define FRAMEDMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEDMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEDM_GLOBAL_H
