#ifndef UDP_GLOBAL_H
#define UDP_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QUdpSocket>

#if defined(UDP_LIBRARY)
#  define UDPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UDPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UDP_GLOBAL_H

#include <string>
#include <map>
