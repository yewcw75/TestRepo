/**
 * @file GjkComponentFactory.h
 * @brief Definition of an abstract GjkComponentFactory class, which is responsible for creating components required
 *  in Gjk class.
 *
 * The GjkComponentFactory class is responsible for creating GJK (Gilbert-Johnson-Keerthi) algorithm components,
 * namely the Simplex and Support objects. It provides abstract methods to create instances of the Simplex and Support classes with the
 * desired threshold for comparing distances, as well as the default threshold value if not specified.
 *
 * The client code can obtain an instance of the GjkComponentFactory using the GjkComponentFactoryCreator class, which helps
 * decouple the creation of GJK components from the client code.
 *
 * @see GjkComponentFactoryCreator, Simplex, Support
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKCOMPONENTFACTORY_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKCOMPONENTFACTORY_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/GjkDefines.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Support.h>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

/**
 * @brief The GjkComponentFactory class is responsible for creating component classes required for Gjk.
 *
 * @see GjkFactoryCreator, Simplex, Support
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST GjkComponentFactory
{
    friend class GjkFactoryCreator;

public:
    /**
     * @brief Default constructor.
     */
    GjkComponentFactory() = default;

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The GjkComponentFactory object to copy from.
     */
    GjkComponentFactory(const GjkComponentFactory& other) = delete;

    /**
     * @brief Virtual destructor.
     */
    virtual ~GjkComponentFactory() = default;

    /**
     * @brief Get an instance of the Simplex object with the specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a Simplex
     *                   vertex/edge. If not set, the default value is defined in GjkDefines.h.
     * @return A pointer to the abstract class Simplex. Client code to take over ownership of pointer.
     */
    virtual Simplex* getSimplex(double eps_square = EPS_SQUARE) = 0;

    /**
     * @brief Get an instance of the Support object with the specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a Support
     *                   vertex/edge. If not set, the default value is defined in GjkDefines.h.
     * @return A pointer to the abstract class Support.  Client code to take over ownership of pointer.
     */
    virtual Support* getSupport(double eps_square = EPS_SQUARE) = 0;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
