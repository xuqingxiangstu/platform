#ifndef CMDTOJSON_GLOBAL_H
#define CMDTOJSON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CMDTOJSON_LIBRARY)
#  define CMDTOJSONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CMDTOJSONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CMDTOJSON_GLOBAL_H