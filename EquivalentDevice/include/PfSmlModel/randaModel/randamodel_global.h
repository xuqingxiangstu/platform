#ifndef RANDAMODEL_GLOBAL_H
#define RANDAMODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RANDAMODEL_LIBRARY)
#  define RANDAMODELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RANDAMODELSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RANDAMODEL_GLOBAL_H