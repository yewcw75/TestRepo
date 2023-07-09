#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SHAPE_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SHAPE_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QDebug>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class RRTPLANNER_LIB_EXPORT IShape
{
    friend class QDebug;
public:
    IShape() = default;
    virtual ~IShape() = default;
    virtual IShape* clone() const = 0;
    virtual VectorF support(const VectorF& dir) const = 0; //return the support point of the shape in the given input direction.

    // Overloading the << operator
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, \
                                                   const RRTPLANNER_NAMESPACE::framework::algorithm::gjk::IShape& data)
    {
        QDebugStateSaver saver(debug);
        debug.nospace() << data.debugPrint();
        return debug;
    }

public:
    virtual QString debugPrint() const = 0; //derived class to define for use in operator<< here
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
