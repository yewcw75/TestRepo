#include <RrtPlannerLib/framework/algorithm/gjk/Polygon.h>
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
    PolygonPrivate()
        :QSharedData(),
          mp_vertexList(new QVector<VectorF>())
    {}
    PolygonPrivate(const std::initializer_list<VectorF>& list)
        :QSharedData(),
          mp_vertexList(new QVector<VectorF>(list))
    {}
    ~PolygonPrivate()
    {}
    PolygonPrivate(const PolygonPrivate& other)
        :QSharedData(other),
          mp_vertexList(new QVector<VectorF>(*other.mp_vertexList))
    {}

public:
    QScopedPointer<QVector<VectorF>> mp_vertexList;

};

//---------
Polygon::Polygon()
    : IShape(),
      mp_pimpl(new PolygonPrivate)
{

}

//---------
Polygon::Polygon(const std::initializer_list<VectorF>& list)
    : IShape(),
      mp_pimpl(new PolygonPrivate(list))
{

}

//---------
Polygon::Polygon(const Polygon& other)
    :IShape(),
      mp_pimpl(other.mp_pimpl)
{


}

//---------
Polygon& Polygon::operator=(const Polygon& other)
{
    if(this != &other){
        this->mp_pimpl = other.mp_pimpl;
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
    return(ret);
}

//----------
VectorF Polygon::support(const VectorF& dir) const
{
    Q_ASSERT(mp_pimpl->mp_vertexList->size() > 0);

    VectorF ret;
    double maxVal = std::numeric_limits<double>::min();
    for(const VectorF& vertex: *mp_pimpl->mp_vertexList){
        double val = VectorFHelper::dot_product(vertex, dir);
        if(val > maxVal){
            ret = vertex;
            maxVal = val;
        }
    }
    return(ret);
}


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
