/**
 * @file SupportMinDist.h
 * @brief Definition of the SupportMinDist class, a concrete implementation of Support class to be used in GjkMinDist.
 *
 * @see GjkMinDist, Support, IShape
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SUPPORT_MINDIST_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SUPPORT_MINDIST_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Support.h>
#include <RrtPlannerLib/framework/VectorF.h>


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class IShape;

/**
 * @brief Definition of the SupportMinDist class, a concrete implementation of Support class to be used in GjkMinDist.@brief Definition of the SupportMinDist class, an extension of Support class to be used in GjkMinDist.
 *
 * @see GjkMinDist, Support, IShape
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST SupportMinDist : public Support
{
public:
    /**
     * @brief Default constructor.
     */
    SupportMinDist();

    /**
     * @brief Constructor with a specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a Support
     *                   vertex/edge. If not set, the default value is 1e-6.
     */
    SupportMinDist(double eps_square);

    /**
     * @brief Virtual destructor.
     */
    virtual ~SupportMinDist();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The SupportMinDist object to copy from.
     */
    SupportMinDist(const SupportMinDist& other) = delete;

    /**
     * @brief Reset the support object used in distance-based GJK algorithm.
     * @details This function clears any internal data in the support object, effectively resetting it for future
     * support point calculations in distance-based GJK.
     */
    virtual void reset() override;

    /**
     * @brief Compute the support point in a given search direction for distance-based GJK calculations.
     * @param shape1 Pointer to the first shape.
     * @param shape2 Pointer to the second shape.
     * @param v The search direction vector.
     * @param isDirFromSupport2Origin True if the search direction, v, is in the direction from any support point of
     *                                 the Minkowski difference to the origin.
     * @param[out] spp The computed support point of the Minkowski difference in the search direction.
     * @return The result flag indicating the relationship of the support point with respect to the simplex and origin.
     */
    virtual Support::ResultFlag support(const IShape* shape1, const IShape* shape2,
                                         const VectorF& v, bool isDirFromSupport2Origin,
                                         VectorF& spp) override;
};

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
