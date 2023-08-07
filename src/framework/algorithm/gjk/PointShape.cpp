#include <RrtPlannerLib/framework/algorithm/gjk/PointShape.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class PointShapePrivate : public QSharedData
{
public:
    PointShapePrivate() = default;
    PointShapePrivate(const PointShapePrivate& other) = default;
    ~PointShapePrivate() = default;

public:
    VectorF m_pt{};

};

//#############################
//----------
PointShape::PointShape()
    : IShape(),
      d_ptr(new PointShapePrivate)
{

}

//----------
PointShape::PointShape(const VectorF& pt)
    :PointShape()
{
    setPt(pt);
}

//----------
PointShape::PointShape(const PointShape& other)
    : IShape(other),
      d_ptr(other.d_ptr)
{

}

//----------
PointShape &PointShape::operator=(const PointShape& other)
{
    if (this != &other){
        IShape::operator=(other);
        d_ptr = other.d_ptr;
    }
    return *this;
}

//----------
PointShape::~PointShape()
{

}

//----------
const VectorF& PointShape::pt() const
{
    return d_ptr->m_pt;
}

//----------
void PointShape::setPt(const VectorF& pt)
{
     d_ptr->m_pt = pt;
}

//----------
IShape* PointShape::clone() const
{
    PointShape* ret = new PointShape(*this);
    ret->detach();
    return(ret);
}

//----------
VectorF PointShape::centroid() const
{
    return(d_ptr->m_pt);
}

//----------
VectorF PointShape::support(const VectorF& dir) const
{
    return(d_ptr->m_pt);
}

//----------
QString PointShape::debugPrint() const
{
   QString ret = QString(" <") + \
           QString::number(d_ptr->m_pt.at(IDX_NORTHING), 'f', 2) + ", " + \
           QString::number(d_ptr->m_pt.at(IDX_EASTING), 'f', 2) + \
           ">";
    return(ret);
}

//----------
void PointShape::detach()
{
    d_ptr.detach();
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
