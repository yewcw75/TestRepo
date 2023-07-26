#include <RrtPlannerLib/framework/Vessel.h>
#include <QDateTime>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VesselPrivate : public QSharedData
{
public:
    VesselPrivate() = default;
    VesselPrivate(const VesselPrivate& other)
        :QSharedData(other),
          m_timeStamp(other.m_timeStamp),
          m_posNE(other.m_posNE),
          m_velNE(other.m_velNE),
          m_hdg_deg(other.m_hdg_deg),
          m_lon0_deg(other.m_lon0_deg)
    {
        if(other.mp_vesShape){
            mp_vesShape.reset(other.mp_vesShape->clone());
        }
    }
    ~VesselPrivate() = default;
public:
    QSharedPointer<VesShape> mp_vesShape{};
    QDateTime m_timeStamp{};
    VectorF m_posNE{};
    VectorF m_velNE{};
    double m_hdg_deg{};
    double m_lon0_deg{};
};

//####################
//----------
Vessel::Vessel()
    : d_ptr(new VesselPrivate)
{

}

//----------
Vessel::Vessel(const VectorF &posNE, double lon0_deg, const VectorF &velNE, double hdg_deg)
    : d_ptr(new VesselPrivate)
{
    setPosNE(posNE, lon0_deg);
    setVelNE(velNE);
    setHdg_deg(hdg_deg);
}

//----------
Vessel::Vessel(const Vessel &rhs)
    : d_ptr{rhs.d_ptr}
{

}

//----------
Vessel &Vessel::operator=(const Vessel &rhs)
{
    if (this != &rhs){
        d_ptr =rhs.d_ptr;
    }
    return *this;
}

//----------
Vessel::~Vessel()
{

}

//----------
void Vessel::setPosNE(const VectorF &posNE, double lon0_deg)
{
    d_ptr->m_posNE = posNE;
    d_ptr->m_lon0_deg = lon0_deg;
}

//----------
double Vessel::lon0_deg() const
{
    return d_ptr->m_lon0_deg;
}

//----------
void Vessel::setLon0_deg(double lon0_deg)
{
    d_ptr->m_lon0_deg = lon0_deg;
}

//----------
const VectorF &Vessel::posNE() const
{
    return d_ptr->m_posNE;
}

//----------
void Vessel::setPosNE(const VectorF &posNE)
{
    d_ptr->m_posNE = posNE;
}

//----------
double Vessel::hdg_deg() const
{
    return d_ptr->m_hdg_deg;
}

//----------
void Vessel::setHdg_deg(double hdg_deg)
{
   d_ptr->m_hdg_deg = hdg_deg;
}

//----------
const VectorF &Vessel::velNE() const
{
    return d_ptr->m_velNE;
}

//----------
void Vessel::setVelNE(const VectorF &velNE)
{
    d_ptr->m_velNE = velNE;
}

//----------
void Vessel::setVesShape(const QSharedPointer<VesShape>& p_vesShape)
{
    d_ptr->mp_vesShape = p_vesShape;
}

//----------
const QSharedPointer<VesShape>& Vessel::vesShape() const
{
    return d_ptr->mp_vesShape;
}

//----------
const QDateTime &Vessel::timeStamp() const
{
    return d_ptr->m_timeStamp;
}

//----------
void Vessel::setTimeStamp(const QDateTime &timeStamp)
{
    d_ptr->m_timeStamp = timeStamp;
}

//----------
void Vessel::setTimeStamp()
{
    d_ptr->m_timeStamp = QDateTime::currentDateTimeUtc();
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
