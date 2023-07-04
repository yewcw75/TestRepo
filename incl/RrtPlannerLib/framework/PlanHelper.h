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
                                     double crossTrackHorizon,
                                     double side,
                                     double eps_dx,
                                     double& dxNearest_out,
                                     QVector<int>& eventSegIdxList_out);

    static Plan getCrossTrackPlan(const Plan& plan,
                                  double crossTrackHorizon, //[m] always a positive variable
                                  double dx, //crosstrack to offset
                                  const QVector<int>& eventSegIdxList, //list of segment indices with edge events at dx
                                  double tol_small,
                                  bool* results_out = nullptr, //results. returns false if error occurred.
                                  QString* results_desc = nullptr //results description
                                  );

    //if side < 0, derived ellmap plans on port side will be preprended successively.
    //if side >= 0, nominal plan will be appended first, followed by successive ellmap plans on stbd.
    //return false if error occurred.
    //results_descr results description
    static bool buildSingleSideEllMaps(const QSharedPointer<Plan> p_planNominal, //nominal plan
                                double side, //-1.0 : port side, 1.0 : stbd side
                                double crossTrackHorizon, //[m] always a positive variable
                                QList<QSharedPointer<Plan>>& planList, //planList to prepend/append
                                QString* results_desc);

    //It is assumed here that if a plan has no missing segment, it will have segments with segment id running from 0 to nSegNominal - 1.
    //When a jump in segment id is found, a zero-length dummy segment with id equals to the missing segment's id will be inserted.
    static void insertDummySegments(QSharedPointer<Plan>& plan, int nSegNominal);

    //if side < 0, plan will be prepended. if side >= 0, plan will be appended.
    static void pushPlan( const QSharedPointer<Plan>& plan,
                            double side,
                            QList<QSharedPointer<Plan>>& planList //planList to prepend/append
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
                                 double dx, //crosstrack to offset
                                 double tol_small
                                 );

};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
