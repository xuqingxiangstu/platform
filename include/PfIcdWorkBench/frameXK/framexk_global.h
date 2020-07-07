#ifndef FRAMEXK_GLOBAL_H
#define FRAMEXK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEXK_LIBRARY)
#  define FRAMEXKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEXKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEXK_GLOBAL_H
