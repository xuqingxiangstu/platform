#ifndef SERIALPORT_GLOBAL_H
#define SERIALPORT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SERIALPORT_LIBRARY)
#  define NISERIALPORTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NISERIALPORTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SERIALPORT_GLOBAL_H
