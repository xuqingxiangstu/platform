#ifndef RECORDLOG_GLOBAL_H
#define RECORDLOG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RECORDLOG_LIBRARY)
#  define RECORDLOGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RECORDLOGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RECORDLOG_GLOBAL_H