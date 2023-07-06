/**
 * @file PlanHelper.h
 * @brief Contains helper functions for the Plan class.
 *
 * @author ycw
 * @date 2023-06-30
 */

#ifndef RRTPLANNER_LIB_PLANHELPER_H
#define RRTPLANNER_LIB_PLANHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/Plan.h>
#include <QObject>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

/**
 * @class PlanHelper
 * @brief Provides helper functions for the Plan class.
 */
class RRTPLANNER_LIB_EXPORT PlanHelper
{
    Q_GADGET
public:
    /**
     * @enum VerifyPlanResult
     * @brief Represents the result of verifying a plan input.
     */
    enum class VerifyPlanResult{
        VERIFY_PLAN_OK = 0,                   ///< The plan input is valid.
        VERIFY_PLAN_ERR_SINGLE_WAYPT,         ///< The plan has less than two waypoints.
        VERIFY_PLAN_ERR_REVERSE_DIR           ///< The segments in the plan do not move in the direction of the first to last waypoint.
    };
    Q_ENUM(VerifyPlanResult)

public:
    /**
     * @brief Default constructor.
     */
    PlanHelper();

    /**
     * @brief Destructor.
     */
    ~PlanHelper();

    /**
     * @brief Verifies that the input to set a plan is feasible for the RRT algorithm.
     * @param wayptList The input waypoint list.
     * @return The verification result indicating if the plan is feasible.
     */
    static VerifyPlanResult verifyPlanInput(const QVector<Waypt>& wayptList);

    /**
     * @brief Finds the nearest edge event on the specified side and also the indices of segments with the edge event.
     * @param plan The input plan to check.
     * @param crossTrackHorizon The maximum cross-track distance to check for edge events. Note: Always a positive number.
     * @param side The side to check: +1.0 for starboard (stbd) or -1.0 for port.
     * @param eps_dx The cross-track range to group edge events.
     * @param dxNearest_out Output parameter to store the cross-track distance of the nearest edge event.
     * @param eventSegIdxList_out Output parameter to store the indices of segments with edge events at dxNearest.
     * @return True if any edge event is found, false otherwise.
     */
    static bool findNearestEdgeEvent(const Plan& plan,
                                     double crossTrackHorizon,
                                     double side,
                                     double eps_dx,
                                     double& dxNearest_out,
                                     QVector<int>& eventSegIdxList_out);

    /**
     * @brief Generates a cross-track plan based on the input plan and edge events.
     * @param plan The input plan.
     * @param crossTrackHorizon The maximum cross-track distance.
     * @param dx The cross-track offset.
     * @param eventSegIdxList The list of segment indices with edge events.
     * @param tol_small A small tolerance value.
     * @param[out] results_out Pointer to store the boolean result of the operation (optional).
     * @param[out] results_desc Pointer to store a description of the operation result (optional).
     * @return The generated cross-track plan.
     */
    static Plan getCrossTrackPlan(const Plan& plan,
                                  double crossTrackHorizon,
                                  double dx,
                                  const QVector<int>& eventSegIdxList,
                                  double tol_small,
                                  bool* results_out = nullptr,
                                  QString* results_desc = nullptr);

    /**
     * @brief Builds offset plans on one side (i.e. port or starboard) of the nominal plan.
     * @param p_planNominal The nominal plan.
     * @param side The side to build EllMap: -1.0 for port, 1.0 for starboard.
     * @param crossTrackHorizon The maximum cross-track distance. Positive number.
     * @param planList The list of plans to store generated plan.
     * @param[out] results_desc Pointer to store a description of the operation result.
     * @return True if the operation is successful, false otherwise.
     */
    static bool buildSingleSideEllMap(const QSharedPointer<Plan> p_planNominal,
                                       double side,
                                       double crossTrackHorizon,
                                       QList<QSharedPointer<Plan>>& planList,
                                       QString* results_desc);

    /**
     * @brief Inserts dummy segments into the plan for fill missing segments.
     * @param plan The plan to modify.
     * @param nSegNominal The number of segments in the nominal plan.
     *
     * Note: The approach assumes that if there are no missing segment, the segment ids should
     * run from 0 to (nSegmentNominal - 1).
     */
    static void insertDummySegments(QSharedPointer<Plan>& plan, int nSegNominal);

    /**
     * @brief Pushes a plan to the planList based on the specified side.
     * @param plan The plan to push.
     * @param side The side to prepend or append the plan: -1.0 for prepend (port side), 1.0 for append (starboard side).
     * @param planList The list of plans to be prepended or appended.
     */
    static void pushPlan(const QSharedPointer<Plan>& plan,
                         double side,
                         QList<QSharedPointer<Plan>>& planList);

    /**
     * @brief Finds an offset point given params as illustrated.
     * @param pt The original point.
     * @param nVec See illustration.
     * @param bVec See illustration.
     * @param dx See illustration.
     * @param tol_small A small tolerance value.
     * @return The offset point.
     *
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
     */
    static VectorF findOffsetWaypt(const VectorF& pt,
                                   const VectorF& nVec,
                                   const VectorF& bVec,
                                   double dx,
                                   double tol_small);
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
