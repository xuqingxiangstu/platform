#ifndef FRAMEC3_GLOBAL_H
#define FRAMEC3_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEC3_LIBRARY)
#  define FRAMEC3SHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEC3SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEC3_GLOBAL_H
