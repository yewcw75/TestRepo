#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SHAPE_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SHAPE_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class RRTPLANNER_LIB_EXPORT IShape
{
public:
    IShape() = default;
    virtual ~IShape() = default;
    virtual IShape* clone() const = 0;
    virtual VectorF support(const VectorF& dir) const = 0; //return the support point of the shape in the given input direction.
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
