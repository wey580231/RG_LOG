#ifndef RG_LOG_GLOBAL_H
#define RG_LOG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RG_LOG_LIBRARY)
#  define RG_LOGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RG_LOGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RG_LOG_GLOBAL_H
