#ifndef PFADAPTERMANAGER_GLOBAL_H
#define PFADAPTERMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PFADAPTERMANAGER_LIBRARY)
#  define PFADAPTERMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PFADAPTERMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PFADAPTERMANAGER_GLOBAL_H
