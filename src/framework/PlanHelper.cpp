#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/UblasHelper.h>
#include <QtGlobal>
#include <QDebug>
#include <limits>

namespace bnu = boost::numeric::ublas;

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//----------
PlanHelper::PlanHelper()
{

}

//----------
PlanHelper::~PlanHelper()
{

}

//----------
PlanHelper::VerifyPlanResult PlanHelper::verifyPlanInput(const QVector<Waypt>& wayptList)
{
    VerifyPlanResult res = VerifyPlanResult::VERIFY_PLAN_OK;
    int nWaypt = wayptList.size();

    //check if there are more than 1 waypt
    res = nWaypt > 1 ? VerifyPlanResult::VERIFY_PLAN_OK : VerifyPlanResult::VERIFY_PLAN_ERR_SINGLE_WAYPT;

    if(res ==  VerifyPlanResult::VERIFY_PLAN_OK){
        //check if any segment goes in a reverse direction
        VectorF wayptPrev = wayptList.at(0).coord_const_ref();
        VectorF wayptLast = wayptList.at(nWaypt-1).coord_const_ref();
        VectorF vecFirstToLast = VectorFHelper::subtract_vector(wayptLast, wayptPrev);
        for(int idx = 1; idx < nWaypt; ++idx){ //loop from 2nd waypt onwards
            VectorF wayptNext = wayptList.at(idx).coord_const_ref();
            VectorF vecPrevToNext = VectorFHelper::subtract_vector(wayptNext, wayptPrev);
            float dotPdt = VectorFHelper::dot_product(vecPrevToNext, vecFirstToLast);
            if(dotPdt < 0.0){
                res = VerifyPlanResult::VERIFY_PLAN_ERR_REVERSE_DIR;
                break;
            }
            wayptPrev = wayptNext; //for next iteration
        }
    }
    return(res);
}

//----------
bool PlanHelper::findNearestEdgeEvent(const Plan& plan,
                                     float crossTrackHorizon,
                                     float side,
                                     float eps_dx,
                                     float& dxNearest_out,
                                     QVector<int>& eventSegIdxList_out)
{
    bool found = false;
    QVector<int> eventSegIdxList;
    float dxNearest = std::numeric_limits<float>::max(); //init dx

    const QVector<Segment>& segmentList = plan.segmentList();
    int nSeg = segmentList.size();

    //edge event will need at least 2 segments
    if(nSeg > 1){
        //check event for each segment
        for(int i = 0; i < nSeg; ++i){
            const Segment& currSeg = segmentList.at(i);
            const VectorF& tVec = currSeg.tVec();
            const VectorF& nVec = currSeg.nVec();
            const VectorF& bVecPrev = currSeg.bVecPrev();
            const VectorF& bVecNext = currSeg.bVecNext();
            const VectorF& wayptPrev = currSeg.wayptPrev().coord_const_ref();
            const VectorF& wayptNext = currSeg.wayptNext().coord_const_ref();
            bnu::matrix M = UblasHelper::concatenate_col_vectors(bVecPrev.data_const_ref(),
                                                                         -1.0 * bVecNext.data_const_ref());
            bnu::vector<float> v = VectorFHelper::subtract_vector(wayptNext, wayptPrev).data_const_ref();
            bnu::vector<float> d;
            bool ok = UblasHelper::solve(M, v, TOL_SMALL, d);
            if(ok){
                bnu::vector<float> posEvent = wayptNext.data_const_ref() + d[1]*bVecNext.data_const_ref();
                bnu::matrix M2 = UblasHelper::concatenate_col_vectors(tVec.data_const_ref(),
                                                                              nVec.data_const_ref());
                bnu::vector<float> v2 = posEvent - wayptPrev.data_const_ref();
                bnu::vector<float> d2;
                bool ok2 = UblasHelper::solve(M2, v2, TOL_SMALL, d2);
                if(ok2){
                    float dx = side * d2[1]; //for port side, cast the problem to stbd side
                    if(dx > 0 && plan.crossTrack() + dx <= crossTrackHorizon){
                       if( dxNearest - dx >  eps_dx){ //=> dx is smaller than dxNearest
                           dxNearest = dx;
                           eventSegIdxList.clear();
                           eventSegIdxList.append(i);
                           found = true;
                       }
                       else if (abs(dxNearest - dx) <= eps_dx){  //=> curr event within close proximity of previous nearest
                           eventSegIdxList.append(i);
                       }
                    }
                } //if(ok2)
            } //if(ok)

        }
    } //if(nSeg > 1)
    dxNearest = side * dxNearest;

    if(found){ //assign output
        dxNearest_out = dxNearest;
        eventSegIdxList_out = eventSegIdxList;
    }
    return(found);
}

//----------
/**
 * @note Developer's note: In matlab version, k_event is used to keep track of the segment index found in eventSegIdxList
 * that we are now looking out for while processing the current input plan's segment.
 * Here, we simply check if the current segment index is contained in eventSegIdxList using QVector::contains() methos.
 * Not expected to be an intensive computation process since eventSegIdxList is not expected to be a super long list.
 *
 */
Plan PlanHelper::getCrossTrackPlan(const Plan& plan,
                                  float crossTrackHorizon,
                                  float dx,
                                  const QVector<int>& eventSegIdxList,
                                  float tol_small,
                                  bool* results_out
                                  )
{
    if(crossTrackHorizon < 0){
        crossTrackHorizon = abs(crossTrackHorizon);
        qWarning() << "[PlanHelper::getCrossTrackPlan] input crossTrackHorizon is negative but should be positive value. Using the absolute value.";
    }

    int nEventSeg = eventSegIdxList.size();
    const QVector<Segment>& segList = plan.segmentList();
    int nSeg = segList.size();

    Plan planOut; //plan to return

    if(nEventSeg < nSeg) {//if no. of segments with events is smaller the number of seg => new plan is not just one point
        QVector<Waypt> wayptList_planOut;
        QVector<int> segIdList_planOut;
        //loop thru ref plan's segment and start building
        for (int idxSeg = 0; idxSeg < nSeg; ++idxSeg){
            //if curr segment has event, dun proceed to form segment
            if(eventSegIdxList.contains(idxSeg)){
                continue;
            }

            const Segment& currSeg = segList.at(idxSeg);
            if(wayptList_planOut.isEmpty()){
                Waypt wayptPrev(currSeg.wayptPrev());
                wayptPrev.setCoord(findOffsetWaypt(currSeg.wayptPrev().coord_const_ref(), currSeg.nVec(), currSeg.bVecPrev(), dx, tol_small));
                wayptList_planOut.append(wayptPrev);
            }
            Waypt wayptNext(currSeg.wayptNext());
            wayptNext.set(findOffsetWaypt(currSeg.wayptNext().coord_const_ref(), currSeg.nVec(), currSeg.bVecNext(), dx, tol_small));
            wayptList_planOut.append(wayptNext);
            segIdList_planOut.append(idxSeg);
        } //for idxSeg = 1: nSeg

        //set planOut
        QString resDesc;
        bool setOk = planOut.setPlan(wayptList_planOut, segIdList_planOut, &resDesc);
        if(results_out){
            *results_out = setOk;
        }
        qInfo() << "[PlanHelper::getCrossTrackPlan] set plan results: " << resDesc;


        planOut.setProperty(Plan::Property::IS_LIMIT, abs(planOut.crossTrack()) >= crossTrackHorizon);
    }
    else { //new plan is a point
        const Segment& firstSeg = segList.first();
        const Segment& lastSeg = segList.last();

        VectorF coord_offset = findOffsetWaypt(firstSeg.wayptPrev().coord_const_ref(), firstSeg.nVec(), firstSeg.bVecPrev(), dx, tol_small);
        Waypt wayptPrev(lastSeg.wayptPrev());
        wayptPrev.setCoord(coord_offset);
        Waypt wayptNext(lastSeg.wayptNext());
        wayptNext.setCoord(coord_offset);
        Segment newSeg(wayptPrev, wayptNext, lastSeg.id(), true);

        planOut.appendSegment(newSeg);
        planOut.setProperty(Plan::Property::IS_LIMIT);
    }
    planOut.setCrossTrack(plan.crossTrack() + dx);
    return(planOut);
}

//----------
VectorF PlanHelper::findOffsetWaypt(const VectorF& pt,
                                    const VectorF& nVec,
                                    const VectorF& bVec,
                                    float dx, //crosstrack to offset
                                    float tol_small
                                    )
{
    //solve scalar, a, using relation: dot(a.bvec, nvec) = dx => a = dx/dot(bvec, nvec)
    float dotPdt = VectorFHelper::dot_product(bVec, nVec);
    bool ok = abs(dotPdt) >= tol_small;
    if(!ok){
        qFatal("[PlanHelper::findOffsetWaypt] Division by zero error");
    }
    float  a = dx/dotPdt;
    VectorF res = VectorFHelper::add_vector(pt, VectorFHelper::multiply_value(bVec, a)); //pt + a * bVec
    return(res);
}

//----------
RRTPLANNER_FRAMEWORK_END_NAMESPACE
