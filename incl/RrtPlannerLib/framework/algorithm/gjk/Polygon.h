#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_POLYGON_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_POLYGON_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class PolygonPrivate;
class RRTPLANNER_LIB_EXPORT Polygon: public IShape
{
public:
    Polygon();
    Polygon(const std::initializer_list<VectorF>& list);
    Polygon(const Polygon& other);
    Polygon& operator=(const Polygon& other);
    virtual ~Polygon();
    virtual Polygon* clone() const override;
    virtual VectorF support(const VectorF& dir) const override; //return the support point of the shape in the given input direction. Note: dir need not be unit vector.

private:
    QSharedDataPointer<PolygonPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif