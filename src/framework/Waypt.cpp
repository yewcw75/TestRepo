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
    :mp_pimpl(new WayptPrivate)
{

}

//----------
Waypt::Waypt(double northing_m, double easting_m, double lon0_deg, int id)
    :mp_pimpl(new WayptPrivate)
{
    set(northing_m, easting_m, lon0_deg, id);
}

//----------
Waypt::Waypt(const VectorF& coord, double lon0_deg, int id)
    :mp_pimpl(new WayptPrivate)
{
    set(coord, lon0_deg, id);
}

//----------
Waypt::~Waypt()
{

}

//----------
Waypt::Waypt(const Waypt& other)
    :mp_pimpl(other.mp_pimpl)
{

}

//----------
Waypt& Waypt::operator=(const Waypt& other)
{
    if(this != &other){
        mp_pimpl = other.mp_pimpl;
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
   return;
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
    mp_pimpl->m_coord[IDX_NORTHING] = northing_m;
    return;
}

//----------
double Waypt::northing() const
{
    return(mp_pimpl->m_coord[IDX_NORTHING]);
}

void Waypt::setEasting(double easting_m)
{
    mp_pimpl->m_coord[IDX_EASTING] = easting_m;
    return;
}

//----------
double Waypt::easting() const
{
    return(mp_pimpl->m_coord[IDX_EASTING]);
}

//----------
void Waypt::setId(int id)
{
    mp_pimpl->m_id = id;
    return;
}

//----------
int Waypt::id() const
{
    return(mp_pimpl->m_id);
}

//----------
void Waypt::setLon0(double lon0_deg)
{
    mp_pimpl->m_lon0_deg = lon0_deg;
    return;
}

//----------
double Waypt::lon0_deg() const
{
    return(mp_pimpl->m_lon0_deg);
}

//----------
void Waypt::setCoord(const VectorF& coord)
{
    mp_pimpl->m_coord = coord;
    return;
}

//----------
const VectorF& Waypt::coord_const_ref() const
{
    return(mp_pimpl->m_coord);
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::Waypt &data)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Id = " << data.id() << ", Coord = " << data.coord_const_ref() << "m, Lon0 = " << data.lon0_deg() << "deg";
    return debug;
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE

