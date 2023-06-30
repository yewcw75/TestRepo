#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/UblasHelper.h>
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
RRTPLANNER_FRAMEWORK_END_NAMESPACE
