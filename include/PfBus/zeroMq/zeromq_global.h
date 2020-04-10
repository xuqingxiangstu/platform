#ifndef ZEROMQ_GLOBAL_H
#define ZEROMQ_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ZEROMQ_LIBRARY)
#  define ZEROMQSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ZEROMQSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ZEROMQ_GLOBAL_H
