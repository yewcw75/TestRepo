#include <RrtPlannerLib/framework/algorithm/gjk/Polygon.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QSharedData>
#include <QVector>
#include <QScopedPointer>
#include <QDebug>
#include <limits>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class PolygonPrivate: public QSharedData
{
public:
    PolygonPrivate() = default;
    PolygonPrivate(const std::initializer_list<VectorF>& list)
        :QSharedData(),
          m_vertexList(list)
    {}   
    PolygonPrivate(const PolygonPrivate& other) = default;
    ~PolygonPrivate() = default;

public:
    QVector<VectorF> m_vertexList;

};

//---------
Polygon::Polygon()
    : IShape(),
      d_ptr(new PolygonPrivate)
{

}

//---------
Polygon::Polygon(const std::initializer_list<VectorF>& list)
    : IShape(),
      d_ptr(new PolygonPrivate(list))
{

}

//---------
Polygon::Polygon(const Polygon& other)
    :IShape(other),
      d_ptr(other.d_ptr)
{


}

//---------
Polygon& Polygon::operator=(const Polygon& other)
{
    if(this != &other){
        IShape::operator=(other);
        this->d_ptr = other.d_ptr;
    }
    return(*this);
}

//----------
Polygon::~Polygon()
{

}

//----------
Polygon* Polygon::clone() const
{
    Polygon* ret = new Polygon(*this);
    ret->detach();
    return(ret);
}

//----------
VectorF Polygon::centroid() const
{
    Q_ASSERT(d_ptr->m_vertexList.size() > 0);

    VectorF ret{0.0, 0.0};
    for(const VectorF& vertex: d_ptr->m_vertexList){
        ret = VectorFHelper::add_vector(ret, vertex);
    }
    ret = VectorFHelper::multiply_value(ret, 1.0/d_ptr->m_vertexList.size());
    return(ret);
}

//----------
VectorF Polygon::support(const VectorF& dir) const
{
    Q_ASSERT(d_ptr->m_vertexList.size() > 0);

    VectorF ret;
    double maxVal = -std::numeric_limits<double>::max();
    for(const VectorF& vertex: d_ptr->m_vertexList){
        double val = VectorFHelper::dot_product(vertex, dir);
        if(val > maxVal){
            ret = vertex;
            maxVal = val;
        }
    }
    return(ret);
}

//----------
int Polygon::size() const
{
    return(d_ptr->m_vertexList.size());
}

//----------
const VectorF& Polygon::at(int i) const
{
    return(d_ptr->m_vertexList.at(i));
}

//----------
VectorF& Polygon::operator[](int i)
{
    return(d_ptr->m_vertexList[i]);
}

//----------
const QVector<VectorF>& Polygon::vertexList_const_ref() const
{
    return(d_ptr->m_vertexList);
}

//----------
QVector<VectorF>& Polygon::vertexList()
{
    return(d_ptr->m_vertexList);
}

//----------
QString Polygon::debugPrint() const
{
    QString ret("[");
    for(const VectorF& vertex: d_ptr->m_vertexList){
        ret += QString(" <") + \
               QString::number(vertex.at(IDX_NORTHING), 'f', 2) + ", " + \
               QString::number(vertex.at(IDX_EASTING), 'f', 2) + \
               ">";
    }
    ret.append(" ]");
    return(ret);
}

//----------
void Polygon::detach()
{
    d_ptr.detach();
}


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
