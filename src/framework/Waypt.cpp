#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/GeometryHelper.h>
#include <QSharedData>
#include <QtGlobal>
#include <QDebug>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class WayptPrivate: public QSharedData
{
public:
    WayptPrivate()
        :QSharedData(),
          m_coord(Coord_NE(DIM_COORD))
    {}
    ~WayptPrivate()
    {}
    WayptPrivate(const WayptPrivate& other)
        : QSharedData(other),
          m_coord(other.m_coord),
          m_fieldFlags(other.m_fieldFlags),
          m_lon0_deg(other.m_lon0_deg),
          m_id(other.m_id)
    {}

public:
    Coord_NE m_coord;
    Waypt::FieldFlags m_fieldFlags{};
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
Waypt::Waypt(float northing_m, float easting_m, double lon0_deg, int id)
    :mp_pimpl(new WayptPrivate)
{
    set(northing_m, easting_m, lon0_deg, id);
}

//----------
Waypt::Waypt(const Coord_NE& coord, double lon0_deg, int id)
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
void Waypt::set(float northing_m, float easting_m, double lon0_deg, int id)
{
   setNorthing(northing_m);
   setEasting(easting_m);
   setLon0(lon0_deg);
   setId(id);
   return;
}

//----------
void Waypt::set(const Coord_NE& coord, double lon0_deg, int id)
{
    setCoord(coord);
    setLon0(lon0_deg);
    setId(id);
}

//----------
void Waypt::setNorthing(float northing_m)
{
    mp_pimpl->m_coord.set<IDX_NORTHING>(northing_m);
    mp_pimpl->m_fieldFlags.setFlag(Waypt::Field::NORTHING);
    return;
}

//----------
float Waypt::northing() const
{
    return(mp_pimpl->m_coord.get<IDX_NORTHING>());
}

void Waypt::setEasting(float easting_m)
{
    mp_pimpl->m_coord.set<IDX_EASTING>(easting_m);
    mp_pimpl->m_fieldFlags.setFlag(Waypt::Field::EASTING);
    return;
}

//----------
float Waypt::easting() const
{
    return(mp_pimpl->m_coord.get<IDX_EASTING>());
}

//----------
void Waypt::setId(int id)
{
    mp_pimpl->m_id = id;
    mp_pimpl->m_fieldFlags.setFlag(Waypt::Field::ID);
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
    mp_pimpl->m_fieldFlags.setFlag(Waypt::Field::LON0);
    return;
}

//----------
double Waypt::lon0_deg() const
{
    return(mp_pimpl->m_lon0_deg);
}

//----------
void Waypt::setCoord(const Coord_NE& coord)
{
    mp_pimpl->m_coord = coord;
    mp_pimpl->m_fieldFlags.setFlag(Waypt::Field::NORTHING);
    mp_pimpl->m_fieldFlags.setFlag(Waypt::Field::EASTING);
    return;
}

//----------
const Coord_NE& Waypt::coord_const_ref() const
{
    return(mp_pimpl->m_coord);
}

//----------
Waypt::FieldFlags Waypt::getFieldFlags() const
{
    return(mp_pimpl->m_fieldFlags);
}

//----------
Vector_NE Waypt::vectorTo(const Waypt& otherWaypt) const
{
    Vector_NE v = GeometryHelper::subtract_point(otherWaypt.coord_const_ref(), this->coord_const_ref());
    return(v);
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE

