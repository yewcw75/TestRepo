/**
 * @file GjkBasic.h
 * @brief Definition of the GjkBasic class, a basic implementation of the GJK algorithm for collision detection, without calculating
 * any distance between input shapes.
 *
 * The GjkBasic class extends the functionality of the Gjk abstract class by implementing the chkIntersect
 * method, which checks for intersection between two shapes. Note that it does not calculate the minimum distance.
 *
 * @see Gjk, IShape
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKBASIC_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_GJKBASIC_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkBasicPrivate;

/**
 * @brief The GjkBasic class provides a basic implementation of the GJK algorithm for collision detection.
 * @details This class is derived from the Gjk abstract class and represents a basic implementation of the
 * Gilbert-Johnson-Keerthi (GJK) algorithm for collision detection. No minimum distance calculation between geometric
 * shapes will be made.
 *
 * @see Gjk, IShape
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST GjkBasic : public Gjk
{
public:
    /**
     * @brief Default constructor.
     */
    GjkBasic();

    /**
     * @brief Virtual destructor.
     */
    virtual ~GjkBasic();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The GjkBasic object to copy from.
     */
    GjkBasic(const GjkBasic& other) = delete;

    /**
     * @brief Check for intersection between two shapes using the GJK algorithm.
     * @param shape1 The first shape.
     * @param shape2 The second shape.
     * @param distance The minimum distance between the shapes if they do not intersect (output parameter).
     * @param isValidDistance Expected to be always false for GjkBasic. This is a flag indicating whether the calculated distance is valid (output parameter).
     *                        If the input shapes intersect, this parameter should return false, as the distance
     *                        is not calculated for intersecting shapes.
     * @return True if the shapes intersect, false otherwise.
     */
    bool chkIntersect(const IShape& shape1, const IShape& shape2, double& distance, bool& isValidDistance);

private:
    QScopedPointer<GjkBasicPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
