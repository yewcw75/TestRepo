#include <RrtPlannerLib/framework/VesShape.h>
#include <QSharedData>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VesShapePrivate : public QSharedData
{
public:
    VesShapePrivate() = default;
    VesShapePrivate(const VesShapePrivate& other) = default;
    ~VesShapePrivate() = default;

public:
    VesShape::Type m_type{VesShape::Type::NOT_SET};
    VectorF m_offset; //[dNorthing, dEasting] in meters
    double m_rotation; //[deg]
};

//#########################
//----------
VesShape::VesShape()
    : d_ptr(new VesShapePrivate)
{

}

//----------
VesShape::VesShape(const Type& type)
    : d_ptr(new VesShapePrivate)
{
    d_ptr->m_type = type;
}

//----------
VesShape::VesShape(const VesShape &other)
    : d_ptr(other.d_ptr)
{

}

//----------
VesShape &VesShape::operator=(const VesShape &other)
{
    if (this != &other){
        d_ptr = other.d_ptr;
    }
    return *this;
}

//----------
VesShape::~VesShape()
{

}

//----------
double VesShape::rotation() const
{
    return d_ptr->m_rotation;
}

//----------
void VesShape::setRotation(double rotation)
{
    d_ptr->m_rotation = rotation;
}

//----------
VesShape::Type VesShape::type() const
{
    return(d_ptr->m_type);
}

//----------
const VectorF& VesShape::offset() const
{
    return d_ptr->m_offset;
}

//----------
void VesShape::setOffset(const VectorF &offset)
{
    d_ptr->m_offset = offset;
}

//----------
void VesShape::detach()
{
    d_ptr.detach();
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
