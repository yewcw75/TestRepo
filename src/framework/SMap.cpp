#include "RrtPlannerLib/framework/EllMap.h"
#include "RrtPlannerLib/framework/SPlan.h"
#include <RrtPlannerLib/framework/SMap.h>
#include <QList>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SMapPrivate : public QSharedData
{
public:
    SMapPrivate() = default;
    SMapPrivate(const SMapPrivate& other) = default;
    ~SMapPrivate() = default;


//    void appendSPlans(const Plan& planPrev, double ellmaxPrev,
//                      const Plan& planNext, double lhNext, double ellmaxNext,
//                      double lh0, double th0, double umin, double umax);

public:
    QList<SPlan> m_SPlanList;
    int m_idxNominal{};
};



//----------
//void SMapPrivate::appendSPlans(const Plan& planPrev, double ellmaxPrev,
//                  const Plan& planNext, double lhNext, double ellmaxNext,
//                  double lh0, double th0, double umin, double umax)
//{
//    //check whether arclength horizon is limited by final arclength at an intermediate crosstrack
//    bool isLimitedOnPrevSide = ellmaxPrev < planPrev.length()  && ellmaxNext > planNext.length();
//    bool isLimitedOnNextSide = ellmaxPrev > planPrev.length()  && ellmaxNext < planNext.length();
//    if (isLimitedOnPrevSide || isLimitedOnNextSide)
//    {
//        //=== Arclength horizon capped at intermediate crosstrack ==============
//        //crosstrack intersection
//        int_dx = planPrev.crosstrack + ((planPrev.length-aux_ellmaxPrev)/((planPrev.length-aux_ellmaxPrev)-(planNext.length-aux_ellmax)))*(planNext.crosstrack-planPrev.crosstrack);

//        xvec  = [ planPrev.crosstrack           int_dx  planNext.crosstrack ];
//        Lhvec = [ sMap.planList(sMap.nPlan).Lh  Lh      aux_Lh              ];

//     }
//    else
//    {

//        //=== Arclength horizon NOT capped at intermediate crosstrack ==========

//        xvec  = [ planPrev.crosstrack           planNext.crosstrack ];
//        Lhvec = [ sMap.planList(sMap.nPlan).Lh  aux_Lh              ];

//    }
//}


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
//void SMap::create(const EllMap& ellMap,
//            const RootData& root_data,
//            double lh0,
//            double th0,
//            double umin,
//            double umax
//            )
//{
//    //first plan
//    auto plan_first = ellMap.at(0);
//    SPlan sPlan;
//    sPlan.setCrosstrack(plan_first->crossTrack());
//    double lh{}, ellMaxPrev{};
//    d_ptr->determineArcLengthHorizon(*plan_first, root_data.ell(), lh0, lh, ellMaxPrev);
//    sPlan.setLh(lh);
//    d_ptr->m_SPlanList.push_back(sPlan);
//    if(plan_first->testProperty(Plan::Property::IS_NOMINAL)){
//        d_ptr->m_idxNominal = 0;
//    }

//}

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

RRTPLANNER_FRAMEWORK_END_NAMESPACE
