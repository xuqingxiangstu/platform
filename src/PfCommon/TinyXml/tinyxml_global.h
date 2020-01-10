#ifndef TINYXML_GLOBAL_H
#define TINYXML_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TINYXML_LIBRARY)
#  define TINYXMLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TINYXMLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TINYXML_GLOBAL_H
