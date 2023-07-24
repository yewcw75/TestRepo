/**
 * @file GjkComponentFactoryMinDist.h
 * @brief Definition of the GjkComponentFactoryMinDist class, a concrete implementation of the GjkComponentFactory.
 *
 * @see GjkComponentFactory, Simplex, Support
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKCOMPONENTFACTORYMINDIST_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKCOMPONENTFACTORYMINDIST_H

#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactory.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Support.h>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

/**
 * @brief Definition of the GjkComponentFactoryMinDist class, a concrete implementation of the GjkComponentFactory.
 *
 * @see GjkComponentFactory, Simplex, Support
 */
class GjkComponentFactoryMinDist : public GjkComponentFactory
{
public:
    /**
     * @brief Default constructor.
     */
    GjkComponentFactoryMinDist();

    /**
     * @brief Virtual destructor.
     */
    virtual ~GjkComponentFactoryMinDist();

    /**
     * @brief Get an instance of the Simplex object with the specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a Simplex
     *                   vertex/edge. If not set, the default value is defined in GjkDefines.h.
     * @return A pointer to the Simplex object. Client code to take over ownership of pointer.
     */
    virtual Simplex* getSimplex(double eps_square) override;

    /**
     * @brief Get an instance of the Support object with the specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a Support
     *                   vertex/edge. If not set, the default value is defined in GjkDefines.h.
     * @return A pointer to the Support object. Client code to take over ownership of pointer.
     */
    virtual Support* getSupport(double eps_square) override;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif

