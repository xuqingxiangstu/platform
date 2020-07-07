#ifndef FRAMEJG_GLOBAL_H
#define FRAMEJG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEJG_LIBRARY)
#  define FRAMEJGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEJGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEJG_GLOBAL_H
