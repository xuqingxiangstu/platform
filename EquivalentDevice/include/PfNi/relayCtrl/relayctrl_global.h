#ifndef RELAYCTRL_GLOBAL_H
#define RELAYCTRL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RELAYCTRL_LIBRARY)
#  define RELAYCTRLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RELAYCTRLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RELAYCTRL_GLOBAL_H