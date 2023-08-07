/**
 * @file SMapHelper.h
 * @brief Contains helper functions for SMap.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-08
 */

#ifndef RRTPLANNER_LIB_SMAPHELPER_H
#define RRTPLANNER_LIB_SMAPHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/EllMap.h>
#include <RrtPlannerLib/framework/Plan.h>
#include <RrtPlannerLib/framework/SMap.h>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

/**
 * @class SMapHelper
 * @brief Provides helper functions for the SMap class.
 */
class RRTPLANNER_LIB_EXPORT SMapHelper
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

    static SMap create(const EllMap& ellMap,       //ellmap input
                const RootData& root_data,  //data relevant to current usv's position
                double lh0,                 //[m] desired arclength horizon
                double th0,                 //[s] desired time horizon
                double umin,                //[m/s] min speed
                double umax                 //[m/s] max speed
                );

    static void appendSPlans(const Plan& planPrev, double ellmaxPrev,
                      const Plan& planNext, double lhNext, double ellmaxNext,
                      double lh0, double th0, double umin, double umax,
                      SMap& sMap);
    static void appendSPlan(const double x_vec[2], const double lh_vec[2], double th0, double umin, double umax, SMap& sMap);


    /**
     * @brief Determine arc length horizon to use for a plan given the arc length corresponding to current usv position.
     * @param plan Input plan.
     * @param ell0 [m] Arclength corresponding to current usv position on input plan.
     * @param lH0 [m] Desired arclength horizon.
     * @param[out] lh [m] Capped arclength horizon.
     * @param[out] ellMax [m] Uncapped arclength horizon coordinate.
     * @return void
     */
    static void determineArcLengthHorizon(const Plan& plan,
                                   double ell0,
                                   double lH0,
                                   double& lh,
                                   double& ellMax
                                   );

    static double sampling_vol3( const double x_vec[2], const double lh_vec[2], const double x_lim[2], double umin, double umax);
    static double sampling_vol4( const double x_vec[2], const double lh_vec[2], const double x_lim[2], double th, double umin, double umax);

};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
