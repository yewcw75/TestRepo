/**
 * @file GjkComponentFactoryCreator.h
 * @brief Definition of the GjkComponentFactoryCreator class, responsible for creating GjkComponentFactory instances.
 *
 * The GjkComponentFactoryCreator class provides a mechanism to obtain concrete instances of the GjkComponentFactory
 * based on the specified GjkType. The GjkComponentFactory is responsible for creating GJK (Gilbert-Johnson-Keerthi)
 * algorithm components, namely the Simplex and Support objects, which are essential for GJK-based collision detection
 * and distance calculation.
 *
 * The GjkType is an enumeration that defines two types of GJK algorithms: Basic and MinDist. The Basic type is used
 * for checking intersection only, while the MinDist type is used for checking intersection and calculating the minimum
 * distance between two shapes if they do not intersect.
 *
 * The GjkComponentFactoryCreator is a static class, and it allows the client code to decouple the creation of GJK
 * components from the client code by providing factory methods for creating the appropriate GjkComponentFactory
 * based on the specified GjkType.
 *
 * @see GjkComponentFactory, GjkType
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKCOMPONENTFACTORY_CREATOR_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKCOMPONENTFACTORY_CREATOR_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/GjkDefines.h>
#include <QObject>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkComponentFactory;
/**
 * @brief The GjkComponentFactoryCreator class provides static factory methods for creating GjkComponentFactory instances
 * based on the specified GjkType.
 *
 * @see GjkComponentFactory, GjkType
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST GjkComponentFactoryCreator
{
    Q_GADGET //for Q_ENUM

public:
    /**
     * @brief The GjkType enumeration defines two types of GJK algorithms: Basic and MinDist.
     * The Basic type is used for checking intersection only, while the MinDist type is used for checking intersection
     * and calculating the minimum distance between two shapes if they do not intersect.
     */
    enum class GjkType
    {
        Basic,   /**< Basic GJK algorithm for checking intersection. */
        MinDist, /**< GJK algorithm for checking intersection and minimum distance calculation. */
    };
    Q_ENUM(GjkType);

    /**
     * @brief Get an instance of the GjkComponentFactory based on the specified GjkType.
     * @param gjkType The GjkType specifying the type of GJK algorithm to use.
     * @return A pointer to the concrete GjkComponentFactory instance corresponding to the specified GjkType.
     *         Client code to take over ownership of pointer.
     */
    static GjkComponentFactory* getGjkComponentFactory(const GjkType& gjkType);
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
