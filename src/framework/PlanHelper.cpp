#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/GeometryHelper.h>
#include <RrtPlannerLib/framework/LinearAlgebraHelper.h>
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
        Coord_NE wayptPrev = wayptList.at(0).coord_const_ref();
        Coord_NE wayptLast = wayptList.at(nWaypt-1).coord_const_ref();
        Vector_NE vecFirstToLast = GeometryHelper::subtract_vector(wayptLast, wayptPrev);
        for(int idx = 1; idx < nWaypt; ++idx){ //loop from 2nd waypt onwards
            Coord_NE wayptNext = wayptList.at(idx).coord_const_ref();
            Vector_NE vecPrevToNext = GeometryHelper::subtract_vector(wayptNext, wayptPrev);
            float dotPdt = GeometryHelper::dot_product(vecPrevToNext, vecFirstToLast);
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
            bnu::vector<float> tVec = LinearAlgebraHelper::to_bnu_vector(currSeg.tVec());
            bnu::vector<float> nVec = LinearAlgebraHelper::to_bnu_vector(currSeg.nVec());
            bnu::vector<float> bVecPrev = LinearAlgebraHelper::to_bnu_vector(currSeg.bVecPrev());
            bnu::vector<float> bVecNext = LinearAlgebraHelper::to_bnu_vector(currSeg.bVecNext());
            bnu::vector<float> wayptPrev = LinearAlgebraHelper::to_bnu_vector(currSeg.wayptPrev().coord_const_ref());
            bnu::vector<float> wayptNext = LinearAlgebraHelper::to_bnu_vector(currSeg.wayptNext().coord_const_ref());
            bnu::matrix M = LinearAlgebraHelper::concatenate_col_vectors(bVecPrev, -1.0*bVecNext);
            bnu::vector<float> v = wayptNext - wayptPrev;
            bnu::vector<float> d;
            bool ok = LinearAlgebraHelper::solve(M, v, TOL_SMALL, d);
            if(ok){
                bnu::vector<float> posEvent = wayptNext + d[1]*bVecNext;
                bnu::matrix M2 = LinearAlgebraHelper::concatenate_col_vectors(tVec, nVec);
                bnu::vector<float> v2 = posEvent - wayptPrev;
                bnu::vector<float> d2;
                bool ok2 = LinearAlgebraHelper::solve(M2, v2, TOL_SMALL, d2);
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
