#include <RrtPlannerLib/framework/ellmap.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h> //for EPS_DX, TOL_SMALL
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <QDebug>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE


class EllMapData : public QSharedData
{
public:
    EllMapData()
        :QSharedData()
    {}
    ~EllMapData()
    {}
    EllMapData(const EllMapData& rhs)
        :QSharedData(rhs),
          m_planSharedPtrList(rhs.m_planSharedPtrList),
          mp_planNominal(rhs.mp_planNominal)
    {}

public:
    QList<QSharedPointer<Plan>> m_planSharedPtrList;
    QSharedPointer<Plan> mp_planNominal;
};

//#################
//----------
EllMap::EllMap()
    : mp_pimpl(new EllMapData)
{

}

//----------
EllMap::EllMap(const EllMap &rhs)
    : mp_pimpl(rhs.mp_pimpl)
{

}

//----------
EllMap &EllMap::operator=(const EllMap &rhs)
{
    if (this != &rhs){
        mp_pimpl.operator=(rhs.mp_pimpl);
    }
    return *this;
}

//----------
EllMap::~EllMap()
{

}

//----------
void EllMap::setNominalPlan(const Plan& plan)
{
    mp_pimpl->mp_planNominal = QSharedPointer<Plan>(new Plan(plan));
    return;
}

//----------
bool EllMap::buildEllMap(double crossTrackHorizon,
                         QString* results_desc)
{
    bool ret = true;
    QString results_desc_local;

    if(mp_pimpl->mp_planNominal) { //pointer is not null
        QList<double> sideList{-1.0, 1.0}; //use in for loop to find ellmaps for port side first, then stbd side.
        for(const auto& side: sideList){
            ret = PlanHelper::buildSingleSideEllMaps(mp_pimpl->mp_planNominal,
                                                    side,
                                                    crossTrackHorizon,
                                                    mp_pimpl->m_planSharedPtrList,
                                                    &results_desc_local);
            if(!ret){
                break; //break for loop and returns if error
            }
        } //for(const auto& side: sideList)
    }
    else{ //mp_pimpl->mp_planNominal is null
        ret = false;
        results_desc_local = QString("[EllMap::buildEllMap] Build EllMap fails. Nominal plan is not set yet.");
    }

    //give the plans an id
    for(int i = 0; i < mp_pimpl->m_planSharedPtrList.size(); ++i){
        mp_pimpl->m_planSharedPtrList.at(i)->setId(i);
    }

    //set output result description if input pointer is not null
    if(results_desc){
        *results_desc = results_desc_local;
    }
    return(ret);
}

//----------
int EllMap::nPlan() const
{
    return(mp_pimpl->m_planSharedPtrList.size());
}

//----------
QSharedPointer<const Plan> EllMap::nominalPlan() const
{
    return(mp_pimpl->mp_planNominal);
}

//----------
QSharedPointer<const Plan> EllMap::at(int idx) const
{
    return(mp_pimpl->m_planSharedPtrList.at(idx));
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::EllMap &data)
{
    QDebugStateSaver saver(debug);
    for(int i = 0; i < data.nPlan(); ++i){
        debug.nospace() << *data.at(i);
    }
    return debug;
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
