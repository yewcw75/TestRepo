#include <RrtPlannerLib/framework/VesRectangle.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
//#define _USE_MATH_DEFINES //for older compiler [?]
#include <math.h> //for M_PI

#define PI2DEG 180.0/M_PI

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VesRectanglePrivate : public QSharedData
{
public:
    VesRectanglePrivate() = default;
    VesRectanglePrivate(const VesRectanglePrivate& other) = default;
    ~VesRectanglePrivate() = default;

public:
    double m_length; //align with y-axis
    double m_width; //align with x-axis
};

//#########################
//----------
VesRectangle::VesRectangle()
    : VesShape(VesShape::Type::RECTANGLE),
      mp_pimpl(new VesRectanglePrivate)
{

}

//----------
VesRectangle::VesRectangle(const VesRectangle &other)
    : VesShape(other),
      mp_pimpl(other.mp_pimpl)
{

}

//----------
VesRectangle &VesRectangle::operator=(const VesRectangle &other)
{
    if (this != &other){
        VesShape::operator=(other);
        mp_pimpl = other.mp_pimpl;
    }
    return *this;
}

//----------
VesRectangle::~VesRectangle()
{

}

//----------
VesShape* VesRectangle::clone() const
{
    VesRectangle* p_ret = new VesRectangle(*this);
    p_ret->detach();
    return(p_ret);
}

//----------
void VesRectangle::setDimensions(double length, //[m] align with y-axis
                   double width   //[m] align with x-axis
                   )
{
    Q_ASSERT(length >= 0.0);
    Q_ASSERT(width >= 0.0);
    mp_pimpl->m_length = length;
    mp_pimpl->m_width = width;
}

//----------
void VesRectangle::setLength(double length)
{
    Q_ASSERT(length >= 0.0);
    mp_pimpl->m_length = length;
}

//----------
double VesRectangle::length() const
{
    return mp_pimpl->m_length;
}

//----------
void VesRectangle::setWidth(double width)
{
    Q_ASSERT(width >= 0.0);
    mp_pimpl->m_width = width;
}

//----------
double VesRectangle::width() const
{
    return mp_pimpl->m_width;
}

//----------
QList<VectorF> VesRectangle::polygon() const
{
    double half_w = mp_pimpl->m_width * 0.5;
    double half_l = mp_pimpl->m_length * 0.5;
    double theta = rotation() * PI2DEG; //rotation in radians
    double sinTheta = sin(theta);
    double cosTheta = cos(theta);
    double dU = offset().at(IDX_U);
    double dV = offset().at(IDX_V);

    //pt in x-y
    QList<VectorF> ptList{VectorF{half_w,   half_l},
                          VectorF{half_w,   -half_l},
                          VectorF{-half_w,  -half_l},
                          VectorF{-half_w,   half_l}};

    //per point
    for(auto& pt: ptList){
        auto x = pt.at(0);
        auto y = pt.at(1);
        //un-rotate to UV coord
        auto u = y*cosTheta - x*sinTheta;
        auto v = y*sinTheta + x*cosTheta;
        pt = VectorF{v - dV, u - dU}; //remove offset
    }

    return ptList;
}

//----------
void VesRectangle::detach()
{
    VesShape::detach();
    mp_pimpl.detach();
}

//----------

RRTPLANNER_FRAMEWORK_END_NAMESPACE
