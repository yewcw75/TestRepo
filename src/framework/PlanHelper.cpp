#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/GeometryHelper.h>
#include <limits>

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
                                     float& dxNearest,
                                     QVector<int>& eventSegIdxList)
{
    bool found = false;
  /*
    eventSegIdxList.clear();
    int nSeg = plan.nSegment();

    float dx = std::numeric_limits<float>::max(); //init dx

    //edge event will need at least 2 segments
    if(nSeg > 1){
        //check event for each segment
    }

    //-----
     %check event for each segment
     for idxSeg = 1 : nSeg
         seg = plan.segList(idxSeg);
         M = [seg.bvecPrev, -seg.bvecNext];
         v = seg.nodeNext -  seg.nodePrev;
         [d, ok] = SolveLinearSyst(M, v, 1e-6);
         if(ok)
             posEvent = seg.nodeNext + d(2) * seg.bvecNext;
             M2 = [seg.tvec, seg.nvec];
             v2 = posEvent - seg.nodePrev;
             [d2, ok2] = SolveLinearSyst(M2, v2, 1e-6);
             if(ok2)
                 dx = side * d2(2); %for port side, cast the problem to stbd side
                 if(dx > 0 && plan.crossTrack + dx <= crossTrackHorizon)
                    if(dxNearest - dx) >  eps_dx %=> dx is smaller than dxNearest
                        dxNearest = dx;
                        nEventSeg = 1;
                        eventSegIdxList(nEventSeg) = idxSeg;
                        found = 1;
                    elseif abs(dxNearest - dx) <= eps_dx %=> curr event within close proximity of previous nearest
                        nEventSeg = nEventSeg + 1;
                        eventSegIdxList(nEventSeg) = idxSeg;
                    end
                 end
             end
         end
     end

     dxNearest = side * dx;
     eventSegIdxList = eventSegIdxList(1:nEventSeg);
     //-----
*/
     return(found);
}

//----------
RRTPLANNER_FRAMEWORK_END_NAMESPACE
