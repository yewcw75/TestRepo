#ifndef RRTPLANNER_LIB_FRAMEWORK_VESRECTANGLE_H
#define RRTPLANNER_LIB_FRAMEWORK_VESRECTANGLE_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VesShape.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QList>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VesRectanglePrivate;
class RRTPLANNER_LIB_EXPORT VesRectangle : public VesShape
{
public:
    VesRectangle();
    VesRectangle(double length, double width);
    VesRectangle(const VesRectangle &other);
    VesRectangle &operator=(const VesRectangle &other);
    virtual ~VesRectangle();

    virtual VesShape* clone() const override;

    void setDimensions(double length, //[m] align with y-axis
                       double width   //[m] align with x-axis
                       );
    void setLength(double length);
    double length() const;
    void setWidth(double width);
    double width() const;

    QList<VectorF> polygon() const; //gets a 4 point polygon (clockwise order) of VesShape in terms of Vessel's coordinates, u-axis (aft) and v-axis (stbd)


protected:
    virtual void detach() override;

private:
    QSharedDataPointer<VesRectanglePrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
