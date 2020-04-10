#ifndef WAVEFORMMODEL_GLOBAL_H
#define WAVEFORMMODEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WAVEFORMMODEL_LIBRARY)
#  define WAVEFORMMODELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WAVEFORMMODELSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WAVEFORMMODEL_GLOBAL_H
    
    