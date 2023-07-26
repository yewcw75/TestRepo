#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QSharedData>
#include <QtGlobal>
#include <QDebug>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class WayptPrivate: public QSharedData
{
public:
    WayptPrivate()
        :QSharedData()
    {
        m_coord.resize(DIM_COORD);
    }
    ~WayptPrivate() = default;
    WayptPrivate(const WayptPrivate& other) = default;

public:
    VectorF m_coord;
    double m_lon0_deg{};
    int m_id{-1};
};

//##############################
//----------
/**
 * @brief Waypt::Waypt
 */
Waypt::Waypt()
    :d_ptr(new WayptPrivate)
{

}

//----------
Waypt::Waypt(double northing_m, double easting_m, double lon0_deg, int id)
    :d_ptr(new WayptPrivate)
{
    set(northing_m, easting_m, lon0_deg, id);
}

//----------
Waypt::Waypt(const VectorF& coord, double lon0_deg, int id)
    :d_ptr(new WayptPrivate)
{
    set(coord, lon0_deg, id);
}

//----------
Waypt::~Waypt()
{

}

//----------
Waypt::Waypt(const Waypt& other)
    :d_ptr(other.d_ptr)
{

}

//----------
Waypt& Waypt::operator=(const Waypt& other)
{
    if(this != &other){
        d_ptr = other.d_ptr;
    }
    return(*this);
}

//----------
void Waypt::set(double northing_m, double easting_m, double lon0_deg, int id)
{
   setNorthing(northing_m);
   setEasting(easting_m);
   setLon0(lon0_deg);
   setId(id);
}

//----------
void Waypt::set(const VectorF& coord, double lon0_deg, int id)
{
    setCoord(coord);
    setLon0(lon0_deg);
    setId(id);
}

//----------
void Waypt::setNorthing(double northing_m)
{
    d_ptr->m_coord[IDX_NORTHING] = northing_m;
}

//----------
double Waypt::northing() const
{
    return(d_ptr->m_coord[IDX_NORTHING]);
}

void Waypt::setEasting(double easting_m)
{
    d_ptr->m_coord[IDX_EASTING] = easting_m;
}

//----------
double Waypt::easting() const
{
    return(d_ptr->m_coord[IDX_EASTING]);
}

//----------
void Waypt::setId(int id)
{
    d_ptr->m_id = id;
}

//----------
int Waypt::id() const
{
    return(d_ptr->m_id);
}

//----------
void Waypt::setLon0(double lon0_deg)
{
    d_ptr->m_lon0_deg = lon0_deg;
}

//----------
double Waypt::lon0_deg() const
{
    return(d_ptr->m_lon0_deg);
}

//----------
void Waypt::setCoord(const VectorF& coord)
{
    d_ptr->m_coord = coord;
}

//----------
const VectorF& Waypt::coord_const_ref() const
{
    return(d_ptr->m_coord);
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::Waypt &data)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Id = " << data.id() << ", Coord = " << data.coord_const_ref() << "m, Lon0 = " << data.lon0_deg() << "deg";
    return debug;
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE

