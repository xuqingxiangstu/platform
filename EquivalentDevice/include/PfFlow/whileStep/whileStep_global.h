#ifndef WHILESTEP_GLOBAL_H
#define WHILESTEP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WHILESTEP_LIBRARY)
#  define WHILESTEPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WHILESTEPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WHILESTEP_GLOBAL_H
    
    