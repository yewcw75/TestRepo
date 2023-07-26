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
    bool m_valid{false};
};

//#########################
//----------
VesShape::VesShape()
    : mp_pimpl(new VesShapePrivate)
{

}

//----------
VesShape::VesShape(const Type& type)
    : mp_pimpl(new VesShapePrivate)
{
    mp_pimpl->m_type = type;
}

//----------
VesShape::VesShape(const VesShape &other)
    : mp_pimpl(other.mp_pimpl)
{

}

//----------
VesShape &VesShape::operator=(const VesShape &other)
{
    if (this != &other){
        mp_pimpl = other.mp_pimpl;
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
    return mp_pimpl->m_rotation;
}

//----------
void VesShape::setRotation(double rotation)
{
    mp_pimpl->m_rotation = rotation;
}

//----------
const VectorF &VesShape::offset() const
{
    return mp_pimpl->m_offset;
}

//----------
void VesShape::setOffset(const VectorF &offset)
{
    mp_pimpl->m_offset = offset;
}

//----------
void VesShape::detach()
{
    mp_pimpl.detach();
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
