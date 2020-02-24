#ifndef ICDDATA_GLOBAL_H
#define ICDDATA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ICDDATA_LIBRARY)
#  define ICDDATASHARED_EXPORT Q_DECL_EXPORT
#else
#  define ICDDATASHARED_EXPORT Q_DECL_IMPORT
#endif

/** 是否采用new **/
#define USE_NEW 0

#endif // ICDDATA_GLOBAL_H
