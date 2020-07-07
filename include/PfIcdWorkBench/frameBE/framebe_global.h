#ifndef FRAMEBE_GLOBAL_H
#define FRAMEBE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEBE_LIBRARY)
#  define FRAMEBESHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEBESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEBE_GLOBAL_H
