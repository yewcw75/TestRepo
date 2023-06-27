/**
 * @file PlanHelper.h
 * Some helper functions for Plan.
 */
#ifndef RRTPLANNER_LIB_PLANHELPER_H
#define RRTPLANNER_LIB_PLANHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/Plan.h>
#include <QObject>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class RRTPLANNER_LIB_EXPORT PlanHelper
{
    Q_GADGET
public:
    enum class VerifyPlanResult{
        VERIFY_PLAN_OK = 0,
        VERIFY_PLAN_ERR_SINGLE_WAYPT,
        VERIFY_PLAN_ERR_REVERSE_DIR
    };
    Q_ENUM(VerifyPlanResult)

public:
    PlanHelper();
    ~PlanHelper();

    /**
     * @brief verifyPlanInput Verify that the input to set a plan is feasible for RRT algorithm.
     * Required conditions:
     *   - At least 2 waypts
     *   - All segments must always move in the direction of first to last waypt [?]
     * @param wayptList Input waypoint list
     * @return bool to indicate if plan is feasible.
     */
    static VerifyPlanResult verifyPlanInput(const QVector<Waypt>& wayptList);

    //static bool findNearestEdgeEvent(const Plan& plan, float crossTrackHorizon, side, float eps_dx);
};


RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
