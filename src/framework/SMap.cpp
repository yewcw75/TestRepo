#include "RrtPlannerLib/framework/SPlan.h"
#include <RrtPlannerLib/framework/SMap.h>
#include <RrtPlannerLib/framework/SMapHelper.h>
#include <RrtPlannerLib/framework/RootData.h>
#include <QList>
#include <QtGlobal>
#include <QDebug>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SMapPrivate : public QSharedData
{
public:
    SMapPrivate() = default;
    SMapPrivate(const SMapPrivate& other) = default;
    ~SMapPrivate() = default;

public:
    QList<SPlan> m_SPlanList;
    RootData m_rootData{};
    EllMap m_ellMap{};
    double m_lh0{};                 //[m] desired arclength horizon
    double m_th0{};                 //[s] desired time horizon
    double m_umin{};                //[m/s] min speed
    double m_umax{};                 //[m/s] max speed
    int m_idxNominal{};
    bool m_ellMapSet{};
};

//######################
//----------
SMap::SMap()
    : d_ptr(new SMapPrivate)
{

}

//----------
SMap::SMap(const SMap& other)
    : d_ptr(other.d_ptr)
{

}

//----------
SMap& SMap::operator=(const SMap& other)
{
    if (this != &other){
        d_ptr = other.d_ptr;
    }
    return *this;
}

//----------
SMap::~SMap()
{

}

//----------
const EllMap& SMap::ellMap() const
{
    return d_ptr->m_ellMap;
}

//----------
void SMap::setEllMap(const EllMap &ellMap,
                     double lh0, //arc-length horizon
                     double th0,  //time-horizon
                     double umin, //min speed
                     double umax  //max speed
                     )
{
    d_ptr->m_ellMap = ellMap;
    d_ptr->m_lh0 = lh0;
    d_ptr->m_th0 = th0;
    d_ptr->m_umin = umin;
    d_ptr->m_umax = umax;
    d_ptr->m_ellMapSet = true;
}

//----------
double SMap::lh0() const
{
    return d_ptr->m_lh0;
}

//----------
double SMap::th0() const
{
    return d_ptr->m_th0;
}

//----------
double SMap::umin() const
{
    return d_ptr->m_umin;
}

//----------
double SMap::umax() const
{
    return d_ptr->m_umax;
}

//----------
bool SMap::reset(const VectorF& posNE)
{
    bool ret = false;
    if(!d_ptr->m_ellMapSet){
        qCritical() << "[SMap::reset] SMap::setEllMap needs to be set first!";
        Q_ASSERT(false);
    }
    else{ //m_ellMapSet ok
        bool foundRoot = d_ptr->m_ellMap.getRootData(posNE, d_ptr->m_rootData);
        if(!foundRoot){
            qCritical() << "[SMap::reset] posNE input is out of bounds of EllMap!";
            Q_ASSERT(false);
        }
        else{ //foundRoot ok!
            SMapHelper::create(d_ptr->m_ellMap,
                               d_ptr->m_rootData,
                               d_ptr->m_lh0,
                               d_ptr->m_th0,
                               d_ptr->m_umin,
                               d_ptr->m_umax,
                               d_ptr->m_SPlanList,
                               d_ptr->m_idxNominal);
            ret = true;
        }
    }
    return ret;
}

//----------
int SMap::size() const
{
    return d_ptr->m_SPlanList.size();
}


//----------
void SMap::append(const SPlan& sPlan)
{
    d_ptr->m_SPlanList.append(sPlan);
}

//----------
const SPlan& SMap::at(int idx) const
{
    return d_ptr->m_SPlanList.at(idx);
}

//----------
SPlan& SMap::operator[](int idx)
{
    return d_ptr->m_SPlanList[idx];
}

//----------
const SPlan& SMap::last() const
{
    return d_ptr->m_SPlanList.last();
}

//----------
SPlan& SMap::last()
{
    return d_ptr->m_SPlanList.last();
}

//----------
const SPlan& SMap::first() const
{
    return d_ptr->m_SPlanList.first();
}

//----------
SPlan& SMap::first()
{
    return d_ptr->m_SPlanList.first();
}

//----------
const QList<SPlan>& SMap::SPlanList_const_ref() const
{
    return d_ptr->m_SPlanList;
}

//----------
QList<SPlan>& SMap::SPlanList()
{
    return d_ptr->m_SPlanList;
}

//----------
void SMap::setSPlanList(const QList<SPlan>& SPlanList)
{
    d_ptr->m_SPlanList = SPlanList;
}

//----------
int SMap::idxNominal() const
{
    return d_ptr->m_idxNominal;
}

//----------
void SMap::setIdxNominal(int idxNominal)
{
    d_ptr->m_idxNominal = idxNominal;
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::SMap &data)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "\nidxNominal = " << data.idxNominal() << "\n" << data.SPlanList_const_ref();
    return debug;
}

//----------
void SMap::setLh0(double lh0)
{
    d_ptr->m_lh0 = lh0;
}

//----------
void SMap::setTh0(double th0)
{
    d_ptr->m_th0 = th0;
}

//----------
void SMap::setUmin(double umin)
{
    d_ptr->m_umin = umin;
}

//----------
void SMap::setUmax(double umax)
{
    d_ptr->m_umax = umax;
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
