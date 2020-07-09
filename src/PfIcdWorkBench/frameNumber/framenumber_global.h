#ifndef FRAMENUMBER_GLOBAL_H
#define FRAMENUMBER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMENUMBER_LIBRARY)
#  define FRAMENUMBERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMENUMBERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMENUMBER_GLOBAL_H