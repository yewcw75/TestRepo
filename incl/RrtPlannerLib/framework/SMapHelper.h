/**
 * @file SMapHelper.h
 * @brief Contains helper functions for the SMap class.
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-08
 */

#ifndef RRTPLANNER_LIB_SMAPHELPER_H
#define RRTPLANNER_LIB_SMAPHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/EllMap.h>
#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/SMap.h>
#include <RrtPlannerLib/framework/SPlan.h>
#include <QList>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

/**
 * @class SMapHelper
 * @brief Provides helper functions for the SMap class.
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST SMapHelper
{
public:
    /**
     * @brief Constructor.
     */
    SMapHelper();

    /**
     * @brief Destructor.
     */
    ~SMapHelper();

    /**
     * @brief Create an SMap based on input parameters and data.
     * @param ellMap The EllMap object.
     * @param root_data The RootData object relevant to the current usv's position.
     * @param lh0 The desired arc-length horizon in meters.
     * @param th0 The desired time horizon in seconds.
     * @param umin The minimum speed in meters per second.
     * @param umax The maximum speed in meters per second.
     * @param[out] sPlanList The list of SPlan objects (output).
     * @param[out] idxNominal The index of the nominal plan in sPlanList (output).
     */
    static void create(const EllMap& ellMap, const RootData& root_data,
                       double lh0, double th0, double umin, double umax,
                       QList<SPlan>& sPlanList, int& idxNominal);

    /**
     * @brief Append SPlans to the sPlanList based on input plans and data.
     * Sub-plans will be created to handle cases where the arc length horizon is capped by the plan length.
     * @param planPrev The previous Plan object.
     * @param ellmaxPrev The maximum arclength in the previous plan.
     * @param planNext The next Plan object.
     * @param lhNext The arclength horizon for the next plan.
     * @param ellmaxNext The maximum arclength in the next plan.
     * @param lh0 The desired arc-length horizon in meters.
     * @param th0 The desired time horizon in seconds.
     * @param umin The minimum speed in meters per second.
     * @param umax The maximum speed in meters per second.
     * @param[out] sPlanList The list of SPlan objects (output).
     */
    static void appendSPlans(const Plan& planPrev, double ellmaxPrev,
                             const Plan& planNext, double lhNext, double ellmaxNext,
                             double lh0, double th0, double umin, double umax,
                             QList<SPlan>& sPlanList);

    /**
     * @brief Append SPlans to the sPlanList based on input data.
     * Sub-plans will be appended to handle cases where time horizon is limited when usv is travelling at min speed.
     * @param x_vec crosstrack for lhs and rhs plans.
     * @param lh_vec The arclength horizon for lhs and rhs plans
     * @param th0 The desired time horizon in seconds.
     * @param umin The minimum speed in meters per second.
     * @param umax The maximum speed in meters per second.
     * @param[out] sPlanList The list of SPlan objects (output).
     */
    static void appendSPlan(const double x_vec[2], const double lh_vec[2],
                            double th0, double umin, double umax,
                            QList<SPlan>& sPlanList);

    /**
     * @brief Determine the arclength horizon and coordinate for a plan.
     * @param plan The input Plan object.
     * @param ell0 The arclength corresponding to the current usv position on the input plan.
     * @param lH0 The desired arclength horizon in meters.
     * @param[out] lh The capped arclength horizon in meters (output).
     * @param[out] ellMax The uncapped arclength horizon coordinate in meters (output).
     */
    static void determineArcLengthHorizon(const Plan& plan,
                                          double ell0,
                                          double lH0,
                                          double& lh,
                                          double& ellMax);

    /**
     * @brief Calculate the volume based on input data.
     * @param x_vec crosstrack of lhs and rhs plans.
     * @param lh_vec The arclength horizon on lhs and rhs plans.
     * @param x_lim The integration limits [?].
     * @param umin The minimum speed in meters per second.
     * @param umax The maximum speed in meters per second.
     * @return The calculated sampling volume.
     */
    static double sampling_vol3(const double x_vec[2], const double lh_vec[2],
                                const double x_lim[2], double umin, double umax);

    /**
     * @brief Calculate the volume based on input data.
     * @param x_vec crosstrack of lhs and rhs plans.
     * @param lh_vec The arclength horizon on lhs and rhs plans.
     * @param x_lim The integration limits [?].
     * @param th The time horizon in seconds.
     * @param umin The minimum speed in meters per second.
     * @param umax The maximum speed in meters per second.
     * @return The calculated sampling volume.
     */
    static double sampling_vol4(const double x_vec[2], const double lh_vec[2],
                                const double x_lim[2], double th, double umin, double umax);
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
