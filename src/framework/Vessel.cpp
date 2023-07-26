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
        if(other.m_vesShape){
            m_vesShape.reset(other.m_vesShape->clone());
        }
    }
    ~VesselPrivate() = default;
public:
    QScopedPointer<VesShape> m_vesShape{};
    QDateTime m_timeStamp{};
    VectorF m_posNE{};
    VectorF m_velNE{};
    double m_hdg_deg{};
    double m_lon0_deg{};
};

//####################
//----------
Vessel::Vessel() : mp_pimpl(new VesselPrivate)
{

}

//----------
Vessel::Vessel(const Vessel &rhs)
    : mp_pimpl{rhs.mp_pimpl}
{

}

//----------
Vessel &Vessel::operator=(const Vessel &rhs)
{
    if (this != &rhs){
        mp_pimpl.operator=(rhs.mp_pimpl);
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
    mp_pimpl->m_posNE = posNE;
    mp_pimpl->m_lon0_deg = lon0_deg;
}

//----------
double Vessel::lon0_deg() const
{
    return mp_pimpl->m_lon0_deg;
}

//----------
void Vessel::setLon0_deg(double lon0_deg)
{
    mp_pimpl->m_lon0_deg = lon0_deg;
}

//----------
const VectorF &Vessel::posNE() const
{
    return mp_pimpl->m_posNE;
}

//----------
void Vessel::setPosNE(const VectorF &posNE)
{
    mp_pimpl->m_posNE = posNE;
}

//----------
double Vessel::hdg_deg() const
{
    return mp_pimpl->m_hdg_deg;
}

//----------
void Vessel::setHdg_deg(double hdg_deg)
{
   mp_pimpl->m_hdg_deg = hdg_deg;
}

//----------
const VectorF &Vessel::velNE() const
{
    return mp_pimpl->m_velNE;
}

//----------
void Vessel::setVelNE(const VectorF &velNE)
{
    mp_pimpl->m_velNE = velNE;
}

//----------
const QDateTime &Vessel::timeStamp() const
{
    return mp_pimpl->m_timeStamp;
}

//----------
void Vessel::setTimeStamp(const QDateTime &timeStamp)
{
    mp_pimpl->m_timeStamp = timeStamp;
}

//----------
void Vessel::setTimeStamp()
{
    mp_pimpl->m_timeStamp = QDateTime::currentDateTimeUtc();
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
