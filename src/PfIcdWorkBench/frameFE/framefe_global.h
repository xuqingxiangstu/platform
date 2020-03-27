#ifndef FRAMEFE_GLOBAL_H
#define FRAMEFE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEFE_LIBRARY)
#  define FRAMEFESHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEFESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEFE_GLOBAL_H
