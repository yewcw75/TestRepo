/**
 * @file Gjk.h
 * Abstract class for GJK algorithm.
 * Use GjkFactory to get the desired version of GJK.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJK_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJK_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QScopedPointer>

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkPrivate;

/**
 * @brief The Gjk class implements the GJK (Gilbert-Johnson-Keerthi) algorithm for collision detection
 * between two shapes.
 */
class RRTPLANNER_LIB_EXPORT Gjk
{
    friend class GjkFactory;

public:
    /**
     * @brief Default constructor.
     */
    Gjk();

    /**
     * @brief Destructor.
     */
    virtual ~Gjk();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The object to copy from.
     */
    Gjk(const Gjk& other) = delete;

    /**
     * @brief Check for intersection between two shapes using the GJK algorithm.
     * @param shape1 The first shape.
     * @param shape2 The second shape.
     * @param distance The minimum distance between the shapes if they do not intersect (output parameter).
     * @param isValidDistance A flag indicating whether the calculated distance is valid (output parameter).
     *                        If the input shapes intersect, this parameter should return false, as the distance is not calculated for intersecting shapes.
     * @return True if the shapes intersect, false otherwise.
     */
    virtual bool chkIntersect(const IShape& shape1, const IShape& shape2,
                              double& distance,
                              bool& isValidDistance) = 0;

    //==============
    //Gjk parameters
    //==============

    /**
     * @brief Set the tolerance to determine if a support point is on the origin or simplex.
     * @param eps_square The tolerance value (default is defined in GjkDefines.h).
     */
    void set_eps_square(double eps_square);

    /**
     * @brief Get the current tolerance value for determining if a support point is on the origin or simplex.
     * @return The current tolerance value.
     */
    double eps_square() const;

    /**
     * @brief Set the maximum number of iterations to run the GJK algorithm.
     * @param max_iteration The maximum number of iterations (default is defined in GjkDefines.h).
     */
    void set_max_iteration(int max_iteration);

    /**
     * @brief Get the current maximum number of iterations for the GJK algorithm.
     * @return The current maximum number of iterations.
     */
    int max_iteration() const;

private:
    QScopedPointer<GjkPrivate> d_ptr; // Private implementation pointer.
};

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
