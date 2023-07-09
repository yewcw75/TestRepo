#ifndef RRPLANNER_LIB_GLOBAL_H
#define RRPLANNER_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RRTPLANNER_LIB_LIBRARY)
#  define RRTPLANNER_LIB_EXPORT Q_DECL_EXPORT
#else
#  define RRTPLANNER_LIB_EXPORT Q_DECL_IMPORT
#endif

#define RRTPLANNER_NAMESPACE    rrtplanner
#define FRAMEWORK_NAMESPACE     framework
#define ALGORITHM_NAMESPACE     algorithm
#define GJK_NAMESPACE           gjk

#define RRTPLANNER_BEGIN_NAMESPACE namespace RRTPLANNER_NAMESPACE{
#define RRTPLANNER_END_NAMESPACE };

#define RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE    namespace RRTPLANNER_NAMESPACE{ \
                                                    namespace FRAMEWORK_NAMESPACE{
#define RRTPLANNER_FRAMEWORK_END_NAMESPACE }; };

#define RRTPLANNER_FRAMEWORK_ALGORITHM_BEGIN_NAMESPACE  namespace RRTPLANNER_NAMESPACE{ \
                                                            namespace FRAMEWORK_NAMESPACE{ \
                                                                namespace  ALGORITHM_NAMESPACE{
#define RRTPLANNER_FRAMEWORK_ALGORITHM_END_NAMESPACE }; }; };

#define RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE  namespace RRTPLANNER_NAMESPACE{ \
                                                                namespace FRAMEWORK_NAMESPACE{ \
                                                                    namespace  ALGORITHM_NAMESPACE{ \
                                                                        namespace GJK_NAMESPACE{
#define RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE }; }; }; };


#endif // RRPLANNER_LIB_GLOBAL_H
