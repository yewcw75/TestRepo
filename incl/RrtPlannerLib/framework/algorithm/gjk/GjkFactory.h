/**
 * @file GjkFactory.h
 * Factory to get implemented Gjk class.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKFACTORY_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKFACTORY_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/GjkDefines.h>
#include <QObject>

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class Gjk;

/**
 * @brief The GjkFactory class is responsible for creating instances of Gjk objects.
 * It provides a factory method for obtaining different types of Gjk algorithms.
 */
class RRTPLANNER_LIB_EXPORT GjkFactory
{
    Q_GADGET //for Q_ENUM
public:
    /**
     * @brief The types of Gjk algorithms supported by the GjkFactory.
     */
    enum class GjkType {
        Basic,    /**< Just checking for intersection. */
        MinDist   /**< Checking for intersection and min dist between two shapes if they do not intersect. */
    };
    Q_ENUM(GjkType);

    /**
     * @brief Get the Gjk algorithm based on the specified GjkType.
     * @param gjkType The type of Gjk algorithm to create (Basic or MinDist).
     * @return A pointer to the Gjk object.
     *
     * The caller is responsible for managing the memory of the returned Gjk object.
     */
    static Gjk* getGjk(const GjkType& gjkType);
};

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
