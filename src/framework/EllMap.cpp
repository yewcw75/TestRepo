#include <RrtPlannerLib/framework/EllMap.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h> //for EPS_DX, TOL_SMALL
#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <RrtPlannerLib/framework/algorithm/gjk/PointShape.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Polygon.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <RrtPlannerLib/framework/algorithm/gjk/GjkFactory.h>
#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <QDebug>




RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

using namespace algorithm::gjk;

class EllMapPrivate : public QSharedData
{
public:
    EllMapPrivate()
        :mp_gjk(GjkFactory::getGjk(GjkFactory::GjkType::Basic))
    {}
    ~EllMapPrivate() = default;
    EllMapPrivate(const EllMapPrivate& rhs);

    bool buildEllMap(double crossTrackHorizon,
                             QString* results_desc);
    bool locateSector(const VectorF& posNE, //usv pos
                      int planIdx_0, int segIdx_0, //initial planIdx and segIdx to start searching
                      int& planIdx, int& segIdx //located sector's associated planIdx and segIdx
                      ) const;

public:
    QList<QSharedPointer<Plan>> m_planSharedPtrList; //plan id to be same as plan idx
    QSharedPointer<Plan> mp_planNominal;
    QScopedPointer<Gjk> mp_gjk;
};

//----------
EllMapPrivate::EllMapPrivate(const EllMapPrivate& rhs)
    : mp_planNominal(QSharedPointer<Plan>(new Plan(*rhs.mp_planNominal))),
      mp_gjk(GjkFactory::getGjk(GjkFactory::GjkType::Basic))
{
    for(const QSharedPointer<Plan>& p_plan_rhs: rhs.m_planSharedPtrList){
        QSharedPointer<Plan> p_plan(new Plan(*p_plan_rhs));
        m_planSharedPtrList.append(p_plan);
    }
}

//----------
bool EllMapPrivate::buildEllMap(double crossTrackHorizon,
                         QString* results_desc)
{
    bool ret = true;
    QString results_desc_local;

    if(mp_planNominal) { //pointer is not null
        QList<double> sideList{-1.0, 1.0}; //use in for loop to find ellmaps for port side first, then stbd side.
        for(const auto& side: sideList){
            ret = PlanHelper::buildSingleSideEllMap(mp_planNominal,
                                                    side,
                                                    crossTrackHorizon,
                                                    m_planSharedPtrList,
                                                    &results_desc_local);
            if(!ret){
                break; //break for loop and returns if error
            }
        } //for(const auto& side: sideList)
    }
    else{ //d_ptr->mp_planNominal is null
        ret = false;
        results_desc_local = QString("[EllMap::buildEllMap] Build EllMap fails. Nominal plan is not set yet.");
    }

    //give the plans an id. //plan id to be same as plan idx
    for(int i = 0; i < m_planSharedPtrList.size(); ++i){
        m_planSharedPtrList.at(i)->setId(i);
    }

    //set output result description if input pointer is not null
    if(results_desc){
        *results_desc = results_desc_local;
    }
    return(ret);
}

//----------
bool EllMapPrivate::locateSector(const VectorF& posNE,
                                 int planIdx_0, int segIdx_0,
                                 int& planIdx, int& segIdx
                                 ) const
{

    bool isInPoly{false};
    int nPlan = m_planSharedPtrList.size();
    int nSeg = mp_planNominal->nSegment();
    algorithm::gjk::PointShape usv(posNE);
    int np = 0;
    int side = 1;

    while (!isInPoly && np < nPlan-1){
        side = -1*side;

        //plan to check
        planIdx = UtilHelper::mod(planIdx_0 + side*static_cast<int>(ceil(0.5*np)), nPlan-1);
        const Plan* planCurr = m_planSharedPtrList.at(planIdx).data();
        const Plan* planRhs = m_planSharedPtrList.at(planIdx+1).data();

        int ns = 0;
        while(ns < nSeg){
            //segment to check
            segIdx = UtilHelper::mod(segIdx_0+ns, nSeg);

            const Segment& segCurr = planCurr->segmentList().at(segIdx);
            const Segment& segRhs = planRhs->segmentList().at(segIdx);

            //sector vertices
            Polygon polysec{segCurr.wayptPrev().coord_const_ref(),
                                       segCurr.wayptNext().coord_const_ref(),
                                       segRhs.wayptPrev().coord_const_ref(),
                                       segRhs.wayptNext().coord_const_ref()};

            //GJK algorithm
            double distance;
            bool isValidDistance;
            isInPoly = mp_gjk->chkIntersect(usv, polysec, distance, isValidDistance);
            Q_ASSERT(!isValidDistance); //using basic Gjk, not expecting distance to be calculated.

            //break if usv is in polysec
            if (isInPoly){
                break;
            }
            ++ns;
        }
        ++np;
    } //while
    return(isInPoly);
}

//#################
//----------
EllMap::EllMap()
    : d_ptr(new EllMapPrivate)
{

}

//----------
EllMap::EllMap(const EllMap& rhs)
    :d_ptr(rhs.d_ptr)
{

}

//----------
EllMap::~EllMap()
{

}

//----------
EllMap& EllMap::operator=(const EllMap& rhs)
{
    if(this != &rhs){
        d_ptr = rhs.d_ptr;
    }
    return(*this);
}

//----------
void EllMap::setNominalPlan(const Plan& plan)
{
    d_ptr->mp_planNominal = QSharedPointer<Plan>(new Plan(plan));
}

//----------
bool EllMap::buildEllMap(double crossTrackHorizon,
                         QString* results_desc)
{
    return(d_ptr->buildEllMap(crossTrackHorizon, results_desc));
}

//----------
int EllMap::nPlan() const
{
    return(d_ptr->m_planSharedPtrList.size());
}

//----------
QSharedPointer<const Plan> EllMap::nominalPlan() const
{
    return(d_ptr->mp_planNominal);
}

//----------
QSharedPointer<const Plan> EllMap::at(int idx) const
{
    return(d_ptr->m_planSharedPtrList.at(idx));
}

//----------
bool EllMap::locateSector(const VectorF& posNE,
                  int planIdx_0, int segIdx_0,
                  int& planIdx, int& segIdx
                  ) const
{
    bool ret = d_ptr->locateSector(posNE, planIdx_0, segIdx_0, planIdx, segIdx);
    return(ret);
}

//----------
bool EllMap::getRootData(const VectorF& posNE, RootData& rootData) const
{
    int planIdx_0 = rootData.planIdx();
    int segIdx_0 = rootData.segIdx();

    int planIdx, segIdx;
    bool ret = d_ptr->locateSector(posNE, planIdx_0, segIdx_0, planIdx, segIdx);
    if(ret){
        auto plan_p = d_ptr->m_planSharedPtrList.at(planIdx);
        auto plan_p_plus_1 = d_ptr->m_planSharedPtrList.at(planIdx + 1);

        //determine crosstrack coordinates
        auto planRef = planIdx < d_ptr->mp_planNominal->id()? \
                    plan_p_plus_1 :
                    plan_p;
        double crossTrack_ref = planRef->crossTrack();
        const VectorF& nodePrev_ref = planRef->segmentList().at(segIdx).wayptPrev().coord_const_ref();
        const VectorF& nVec_ref = planRef->segmentList().at(segIdx).nVec();
        VectorF dPos = VectorFHelper::subtract_vector(posNE, nodePrev_ref);
        double dx_ref = VectorFHelper::dot_product(dPos, nVec_ref);
        double dx = dx_ref + crossTrack_ref;

        //get offset plan at posNE position
        bool results_out;
        QString results_desc;
        Plan planOffset = PlanHelper::getCrossTrackPlan(*planRef, 2.0*abs(dx_ref), dx_ref, QVector<int>(), TOL_SMALL, &results_out, &results_desc);
        if(!results_out){
            qCritical() << "[EllMap::getRootData] error getting cross track plan: " << results_desc;
            Q_ASSERT(false);
        }

        //parameters for offset plan at dx
        double cumLength = segIdx > 0? planOffset.segmentList().at(segIdx - 1).lengthCumulative() : 0.0;
        const Segment& segOffset = planOffset.segmentList().at(segIdx);
        const VectorF& nodePrev = segOffset.wayptPrev().coord_const_ref();
        double d_ell = VectorFHelper::norm2(VectorFHelper::subtract_vector(posNE, nodePrev));
        double L = segOffset.length();
        double f_ell = d_ell/L;

        rootData.setDx(dx);
        rootData.setEll(cumLength + d_ell);
        rootData.setL(L);
        rootData.setF_ell(f_ell);

        rootData.setPlanIdx(planIdx);
        rootData.setSegIdx(segIdx);
        rootData.setPosNE(posNE);
        rootData.setIsInPoly(true);

        //USV arclength baseline. Set ell_list
        rootData.ell_list().clear();
        for(const auto& plan: d_ptr->m_planSharedPtrList){
            double cumLength_curr = segIdx > 0? plan->segmentList().at(segIdx - 1).lengthCumulative() : 0.0;
            double ell_curr= cumLength_curr + f_ell * plan->segmentList().at(segIdx).length();
            rootData.ell_list().append(ell_curr);
        }


//        dx += crossTrack_ref;
//
//        //determine initial and final segment waypoints for determined crosstrack
//        double fx   = (dx-plan_p->crossTrack())/(plan_p_plus_1->crossTrack()-plan_p->crossTrack());
//        const VectorF& nodePrev_p = plan_p->segmentList().at(segIdx).wayptPrev().coord_const_ref();
//        const VectorF& nodeNext_p = plan_p->segmentList().at(segIdx).wayptNext().coord_const_ref();
//        const VectorF& nodePrev_p_plus_1 = plan_p_plus_1->segmentList().at(segIdx).wayptPrev().coord_const_ref();
//        const VectorF& nodeNext_p_plus_1 = plan_p_plus_1->segmentList().at(segIdx).wayptNext().coord_const_ref();
//        VectorF p0 = VectorFHelper::add_vector( VectorFHelper::multiply_value(nodePrev_p, 1-fx),
//                                                VectorFHelper::multiply_value(nodePrev_p_plus_1, fx) );
//        VectorF pF = VectorFHelper::add_vector( VectorFHelper::multiply_value(nodeNext_p, 1-fx),
//                                                VectorFHelper::multiply_value(nodeNext_p_plus_1, fx) );

//        //determine arclength from segment s
//        double dell = VectorFHelper::norm2(VectorFHelper::subtract_vector(posNE, p0));
    }
    else{
        rootData.reset();
    }
    rootData.setPosNE(posNE);
    return(ret);
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
