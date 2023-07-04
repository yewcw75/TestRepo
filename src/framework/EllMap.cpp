#include <RrtPlannerLib/framework/ellmap.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h> //for EPS_DX, TOL_SMALL
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <QList>
#include <QSharedPointer>
#include <QString>

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
          mp_planNominal(rhs.mp_planNominal),
          m_fieldFlags(rhs.m_fieldFlags)
    {}

public:
    QList<QSharedPointer<Plan>> m_planSharedPtrList;
    QSharedPointer<Plan> mp_planNominal;
    EllMap::FieldFlags m_fieldFlags{};
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
    mp_pimpl->m_fieldFlags.setFlag(EllMap::Field::NOMINAL_PLAN);
    mp_pimpl->m_fieldFlags.setFlag(EllMap::Field::ELLMAPS, false);
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
            ret = PlanHelper::buildSuccessiveEllMap(mp_pimpl->mp_planNominal,
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

    if(results_desc){
        *results_desc = results_desc_local;
    }
    return(ret);
}

//----------
const EllMap::FieldFlags &EllMap::getFieldFlags() const
{
    return(mp_pimpl->m_fieldFlags);
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
