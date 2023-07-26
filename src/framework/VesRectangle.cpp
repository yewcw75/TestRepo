#include <RrtPlannerLib/framework/VesRectangle.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
//#define _USE_MATH_DEFINES //for older compiler [?]
#include <math.h> //for M_PI

#define DEG2RAD M_PI/180.0

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
      d_ptr(new VesRectanglePrivate)
{

}

//----------
VesRectangle::VesRectangle(double length, double width)
    : VesShape(VesShape::Type::RECTANGLE),
      d_ptr(new VesRectanglePrivate)
{
    d_ptr->m_length = length;
    d_ptr->m_width = width;
}

//----------
VesRectangle::VesRectangle(const VesRectangle &other)
    : VesShape(other),
      d_ptr(other.d_ptr)
{

}

//----------
VesRectangle &VesRectangle::operator=(const VesRectangle &other)
{
    if (this != &other){
        VesShape::operator=(other);
        d_ptr = other.d_ptr;
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
    setLength(length);
    setWidth(width);
}

//----------
void VesRectangle::setLength(double length)
{
    Q_ASSERT(length >= 0.0);
    d_ptr->m_length = length;
}

//----------
double VesRectangle::length() const
{
    return d_ptr->m_length;
}

//----------
void VesRectangle::setWidth(double width)
{
    Q_ASSERT(width >= 0.0);
    d_ptr->m_width = width;
}

//----------
double VesRectangle::width() const
{
    return d_ptr->m_width;
}

//----------
QList<VectorF> VesRectangle::polygon() const
{
    double half_w = d_ptr->m_width * 0.5;
    double half_l = d_ptr->m_length * 0.5;
    double theta = rotation() * DEG2RAD; //rotation in radians
    double sinTheta = sin(theta);
    double cosTheta = cos(theta);
    double dU = offset().at(IDX_U);
    double dV = offset().at(IDX_V);

    //pt in x-y
    QList<VectorF> ptList{VectorF{half_w,   half_l},
                          VectorF{half_w,   -half_l},
                          VectorF{-half_w,  -half_l},
                          VectorF{-half_w,   half_l}}; //in x-y coord

   //qInfo() << "xy" << ptList;

    //per point
    for(auto& pt: ptList){
        auto x = pt.at(0);
        auto y = pt.at(1);
        //un-rotate to UV coord
        auto u = y*cosTheta - x*sinTheta;
        auto v = y*sinTheta + x*cosTheta;
        pt = VectorF{u + dU, v + dV}; //add offset
    }

    //qInfo() << ptList;

    return ptList;
}

//----------
void VesRectangle::detach()
{
    VesShape::detach();
    d_ptr.detach();
}

//----------

RRTPLANNER_FRAMEWORK_END_NAMESPACE
