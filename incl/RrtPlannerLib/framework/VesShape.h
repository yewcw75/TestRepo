//A base class to define vessel shape.
//A vessels coordinates are assumed to be:
//   - origin at boat center.
//   - u-axis positive towards aft.
//   - v-axis positive towards starboard.
//An unrotated and untranslated VesShape's coordinates are aligned with that of the vessel coordinates:
//   - origin at boat center
//   - y-axis align with u-axis
//   - x-axis align with v-axis
//Translation in [dU, dV] can be set.
//Rotation in degrees can be set.
#ifndef RRTPLANNER_LIB_FRAMEWORK_VESSHAPE_H
#define RRTPLANNER_LIB_FRAMEWORK_VESSHAPE_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QObject> //for Q_GADGET
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VesShapePrivate;
class RRTPLANNER_LIB_EXPORT VesShape
{
    Q_GADGET //for Q_ENUM
public:
    enum class Type{
        NOT_SET,
        RECTANGLE,
        //POLYGON
    };
    Q_ENUM(Type);

public:
    VesShape();
    VesShape(const Type& type);
    VesShape(const VesShape &other);
    VesShape &operator=(const VesShape &other);
    virtual ~VesShape();

    virtual VesShape* clone() const = 0;

    Type type() const;
    const VectorF &offset() const;
    void setOffset(const VectorF &offset); //[dNorthing; dEasting] in metres
    double rotation() const;
    void setRotation(double rotation); //[deg] 0 => aligned with vessel heading. clockwise positive.

protected:
    virtual void detach();

private:
    QSharedDataPointer<VesShapePrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif


