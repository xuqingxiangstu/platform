#ifndef QTPROPERTYBROWSER_GLOBAL_H
#define QTPROPERTYBROWSER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTPROPERTYBROWSER_LIBRARY)
#  define QTPROPERTYBROWSERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTPROPERTYBROWSERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTPROPERTYBROWSER_GLOBAL_H