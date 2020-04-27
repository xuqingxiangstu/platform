#ifndef M1553B_GLOBAL_H
#define M1553B_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(M1553B_LIBRARY)
#  define M1553BSHARED_EXPORT Q_DECL_EXPORT
#else
#  define M1553BSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // M1553B_GLOBAL_H
