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

    /**
     * @brief findNearestEdgeEvent Finds the nearest edge event, dxNearest, on the specified side,
     * and also indices of segments with edge event at dxNearest.
     * @param plan Input plan to check.
     * @param crossTrackHorizon [m] Maximum cross track distance to check for edge event.
     * @param side +1.0: stbd, -1.0 port.
     * @param eps_dx [m] cross track range to group edge events.
     * @param[out] dxNearest_out [m] Cross track of the nearest edge event.
     * @param[out] eventSegIdxList_out List containing indices of segments with edge event at dxNearest.
     * The indices will be in increasing order. Note that the index of the first segment is 0.
     * @return bool to indicate if any edge event is found.
     */
    static bool findNearestEdgeEvent(const Plan& plan,
                                     float crossTrackHorizon,
                                     float side,
                                     float eps_dx,
                                     float& dxNearest_out,
                                     QVector<int>& eventSegIdxList_out);

    static Plan getCrossTrackPlan(const Plan& plan,
                                  float crossTrackHorizon, //[m] always a positive variable
                                  float dx, //crosstrack to offset
                                  const QVector<int>& eventSegIdxList, //list of segment indices with edge events at dx
                                  float tol_small,
                                  bool* results_out = nullptr //results
                                  );

    /**
     * @brief findOffsetWaypt Find an offset point, pt_offset, along unit vector bVec such that
     * the it is dx away from the original point, pt, along nVec direction vector.
     * @param tol_small a small number, e.g. 1e-6, to detect divison-by-zero.
     * @return Offset point
     *  *
     *              ^ tvec
     *              |
     *              |
     *              -----> nvec
     *
     *
     *        pt     @-----dx.nVec->--
     *               \             |_|90 deg
     *                   \           |
     *                      \        |
     *                 a.bVec  \     |
     *                            \  |
     *                              @ pt_offset
     *
     *   pt - pt_offset = a.bVec
     *
     */
    static VectorF findOffsetWaypt(const VectorF& pt,
                                 const VectorF& nVec,
                                 const VectorF& bVec,
                                 float dx, //crosstrack to offset
                                 float tol_small
                                 );

};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
