/**
 * @file GjkMinDist.h
 * @brief Definition of the GjkMinDist class, providing a GJK-based algorithm for finding the minimum distance between shapes.
 *
 * This header file defines the GjkMinDist class, which is derived from the Gjk abstract class. The GjkMinDist class
 * implements a Gilbert-Johnson-Keerthi (GJK) algorithm to check for intersection and calculate the minimum distance if
 * the shapes do not intersect.
 *
 * @see Gjk, IShape
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKMINDIST_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKMINDIST_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkMinDistPrivate;

/**
 * @brief The GjkMinDist class provides a GJK-based algorithm for checking if shapes intersect,
 *   and for finding the minimum distance between shapes if they do not intersect.
 *
 * @see Gjk, IShape
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST GjkMinDist : public Gjk
{
public:
    /**
     * @brief Default constructor.
     */
    GjkMinDist();

    /**
     * @brief Virtual destructor.
     */
    virtual ~GjkMinDist();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The GjkMinDist object to copy from.
     */
    GjkMinDist(const GjkMinDist& other) = delete;

    /**
     * @brief Check for intersection between two shapes and calculate the minimum distance.
     * @param shape1 The first shape.
     * @param shape2 The second shape.
     * @param distance The minimum distance between the shapes if they do not intersect (output parameter).
     * @param isValidDistance A flag indicating whether the calculated distance is valid (output parameter).
     *                        If the input shapes intersect, this parameter should return false, as the distance
     *                        is not calculated for intersecting shapes.
     * @return True if the shapes intersect, false otherwise.
     */
    bool chkIntersect(const IShape& shape1, const IShape& shape2, double& distance, bool& isValidDistance);

private:
    QScopedPointer<GjkMinDistPrivate> d_ptr;
};

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
