#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/GeometryHelper.h>

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
        Vector_NE vecFirstToLast = GeometryHelper::subtractVector(wayptLast, wayptPrev);
        for(int idx = 1; idx < nWaypt; ++idx){ //loop from 2nd waypt onwards
            Coord_NE wayptNext = wayptList.at(idx).coord_const_ref();
            Vector_NE vecPrevToNext = GeometryHelper::subtractVector(wayptNext, wayptPrev);
            float dotPdt = GeometryHelper::dotProductVector(vecPrevToNext, vecFirstToLast);
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
RRTPLANNER_FRAMEWORK_END_NAMESPACE
