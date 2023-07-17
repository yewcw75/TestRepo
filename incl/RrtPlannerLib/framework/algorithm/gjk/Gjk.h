#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJK_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJK_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkPrivate;
class RRTPLANNER_LIB_EXPORT Gjk
{
public:
    Gjk();
    virtual ~Gjk();
    Gjk(const Gjk& other) = delete; //non-copyable

    static bool chkIntersect(const IShape& shape1, const IShape& shape2);

private:
    static QScopedPointer<GjkPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
