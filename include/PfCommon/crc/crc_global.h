#ifndef CRC_GLOBAL_H
#define CRC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CRC_LIBRARY)
#  define CRCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CRCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CRC_GLOBAL_H
