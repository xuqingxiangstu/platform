#ifndef FRAMEMIDDLE_GLOBAL_H
#define FRAMEMIDDLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FRAMEMIDDLE_LIBRARY)
#  define FRAMEMIDDLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEMIDDLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FRAMEMIDDLE_GLOBAL_H
