#ifndef ELSESTEP_GLOBAL_H
#define ELSESTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ELSESTEP_LIBRARY)
#  define ELSESTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ELSESTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ELSESTEP_GLOBAL_H
    
    