#ifndef FRAME93_GLOBAL_H
#define FRAME93_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAME93_LIBRARY)
#  define FRAME93SHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAME93SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAME93_GLOBAL_H
