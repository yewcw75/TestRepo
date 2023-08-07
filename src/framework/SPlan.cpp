#include <RrtPlannerLib/framework/SPlan.h>
#include <QSharedData>
#include <QtGlobal>
#include <QDebug>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SPlanPrivate: public QSharedData
{
public:
    SPlanPrivate() = default;
    ~SPlanPrivate() = default;
    SPlanPrivate(const SPlanPrivate& other) = default;

public:
    double m_crosstrack{};    // crosstrack coordinate
    double m_lH{};            // local arclength horizon
    double m_vol_cum{};       // cumulative volume
    double m_area_cum{};      // cumulative area
};

//##########################
//----------
SPlan::SPlan()
    :d_ptr(new SPlanPrivate())
{

}

//----------
SPlan::SPlan(double crossTrack, double Lh, double vol_cum, double area_cum)
    :SPlan()
{
    setCrosstrack(crossTrack);
    setLh(Lh);
    setVol_cum(vol_cum);
    setArea_cum(area_cum);
}

//----------
SPlan::~SPlan()
{

}

//----------
SPlan::SPlan(const SPlan& other)
    :d_ptr(other.d_ptr)
{

}

//----------
SPlan& SPlan::operator=(const SPlan& other)
{
    if(this != &other){
        d_ptr = other.d_ptr;
    }
    return(*this);
}

//----------
double SPlan::getLh() const
{
    return d_ptr->m_lH;
}

//----------
void SPlan::setLh(double lh)
{
    d_ptr->m_lH = lh;
}

//----------
double SPlan::getVol_cum() const
{
    return d_ptr->m_vol_cum;
}

//----------
void SPlan::setVol_cum(double vol_cum)
{
    d_ptr->m_vol_cum = vol_cum;
}

//----------
double SPlan::getArea_cum() const
{
    return d_ptr->m_area_cum;
}

//----------
void SPlan::setArea_cum(double area_cum)
{
    d_ptr->m_area_cum = area_cum;
}

//----------
double SPlan::getCrosstrack() const
{
    return d_ptr->m_crosstrack;
}

//----------
void SPlan::setCrosstrack(double crosstrack)
{
    d_ptr->m_crosstrack = crosstrack;
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::SPlan &data)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "\n  crossTrack = " << data.getCrosstrack() << \
                       ", lH = " << data.getLh() << \
                       ", vol_cum = " << data.getVol_cum() << \
                       ", area_cum = " << data.getArea_cum();
    return debug;
}


RRTPLANNER_FRAMEWORK_END_NAMESPACE
