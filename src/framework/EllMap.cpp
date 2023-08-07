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

    bool buildEllMap(Plan planNominal, //make a copy because we want to ensure some settings of property flags inside the function.
                     double crossTrackHorizon,
                     QString* results_desc);
    bool locateSector(const VectorF& posNE, //usv pos
                      int planIdx_0, int segIdx_0, //initial planIdx and segIdx to start searching
                      int& planIdx, int& segIdx //located sector's associated planIdx and segIdx
                      ) const;
    Plan planNominal() const; //get nominal plan

public:
    QList<Plan> m_planList; //plan id to be same as plan idx
    QScopedPointer<Gjk> mp_gjk;
    int m_idxNominal{-1};
    bool m_ellMapReady{};
};

//----------
EllMapPrivate::EllMapPrivate(const EllMapPrivate& rhs)
    : QSharedData(rhs),
      m_planList(rhs.m_planList),
      mp_gjk(GjkFactory::getGjk(GjkFactory::GjkType::Basic)),
      m_idxNominal(rhs.m_idxNominal),
      m_ellMapReady(rhs.m_ellMapReady)
{

}

//----------
bool EllMapPrivate::buildEllMap(Plan planNominal,
                                double crossTrackHorizon,
                                QString* results_desc)
{
    m_ellMapReady = true;
    QString results_desc_local;
    planNominal.setProperty(Plan::Property::IS_NOMINAL); //ensure property is set properly.

    QList<double> sideList{-1.0, 1.0}; //use in for loop to find ellmaps for port side first, then stbd side.
    for(const auto& side: sideList){
        m_ellMapReady = PlanHelper::buildSingleSideEllMap(planNominal,
                                                side,
                                                crossTrackHorizon,
                                                m_planList,
                                                &results_desc_local);
        if(!m_ellMapReady){
            break; //break for loop and returns if error
        }
    } //for(const auto& side: sideList)

    //give the plans an id. //plan id to be same as plan idx.
    //set m_idxNominal
    for(int i = 0; i < m_planList.size(); ++i){
        m_planList[i].setId(i);
        if(m_planList[i].testProperty(Plan::Property::IS_NOMINAL)){
            m_idxNominal = i;
        }
    }

    //set output result description if input pointer is not null
    if(results_desc){
        *results_desc = results_desc_local;
    }
    return(m_ellMapReady);
}

//----------
bool EllMapPrivate::locateSector(const VectorF& posNE,
                                 int planIdx_0, int segIdx_0,
                                 int& planIdx, int& segIdx
                                 ) const
{
    if(!m_ellMapReady){
        qCritical() << "[EllMapPrivate::locateSector] cannot call this function before EllMapPrivate::buildEllMap() is called successfully!";
        Q_ASSERT(false);
    }

    bool isInPoly{false};
    int nPlan = m_planList.size();
    int nSeg = planNominal().nSegment();
    algorithm::gjk::PointShape usv(posNE);
    int np = 0;
    int side = 1;

    while (!isInPoly && np < nPlan-1){
        side = -1*side;

        //plan to check
        planIdx = UtilHelper::mod(planIdx_0 + side*static_cast<int>(ceil(0.5*np)), nPlan-1);
        const Plan& planCurr = m_planList.at(planIdx);
        const Plan& planRhs = m_planList.at(planIdx+1);

        int ns = 0;
        while(ns < nSeg){
            //segment to check
            segIdx = UtilHelper::mod(segIdx_0+ns, nSeg);

            const Segment& segCurr = planCurr.segmentList().at(segIdx);
            const Segment& segRhs = planRhs.segmentList().at(segIdx);

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

//----------
Plan EllMapPrivate::planNominal() const
{
    Plan ret;
    if(!m_ellMapReady){
        qCritical() << "[EllMapPrivate::locateSector] cannot call this function before EllMapPrivate::buildEllMap() is called successfully!";
        Q_ASSERT(false);
    }
    else{
        ret = m_planList.at(m_idxNominal);
    }
    return ret;
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
bool EllMap::buildEllMap(Plan plan,
                         double crossTrackHorizon,
                         QString* results_desc)
{
    return(d_ptr->buildEllMap(plan, crossTrackHorizon, results_desc));
}

//----------
int EllMap::size() const
{
    return d_ptr->m_planList.size();
}

//----------
Plan EllMap::planNominal() const
{
    return d_ptr->planNominal();
}

//----------
const Plan& EllMap::at(int idx) const
{
    return d_ptr->m_planList.at(idx);
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
        const Plan& plan_p = d_ptr->m_planList.at(planIdx);
        const Plan& plan_p_plus_1 = d_ptr->m_planList.at(planIdx + 1);

        //determine crosstrack coordinates
        const Plan& planRef = planIdx < d_ptr->m_idxNominal? \
                    plan_p_plus_1 :
                    plan_p;
        double crossTrack_ref = planRef.crossTrack();
        const VectorF& nodePrev_ref = planRef.segmentList().at(segIdx).wayptPrev().coord_const_ref();
        const VectorF& nVec_ref = planRef.segmentList().at(segIdx).nVec();
        VectorF dPos = VectorFHelper::subtract_vector(posNE, nodePrev_ref);
        double dx_ref = VectorFHelper::dot_product(dPos, nVec_ref);
        double dx = dx_ref + crossTrack_ref;

        //get offset plan at posNE position
        bool results_out;
        QString results_desc;
        Plan planOffset = PlanHelper::getCrossTrackPlan(planRef, 2.0*abs(dx_ref), dx_ref, QVector<int>(), TOL_SMALL, &results_out, &results_desc);
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
        for(const Plan& plan: d_ptr->m_planList){
            double cumLength_curr = segIdx > 0? plan.segmentList().at(segIdx - 1).lengthCumulative() : 0.0;
            double ell_curr= cumLength_curr + f_ell * plan.segmentList().at(segIdx).length();
            rootData.ell_list().append(ell_curr);
        }
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
    for(int i = 0; i < data.size(); ++i){
        debug.nospace() << data.at(i);
    }
    return debug;
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
