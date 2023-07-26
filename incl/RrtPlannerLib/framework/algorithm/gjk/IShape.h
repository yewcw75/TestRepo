/**
 * @file IShape.h
 * @brief Interface class for a Shape class to be passed into the Gjk class.
 * @details This file contains the interface class for a shape to be used in the Gilbert-Johnson-Keerthi (GJK) algorithm.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */

#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SHAPE_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SHAPE_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QDebug>

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

/**
 * @brief The IShape class is an interface for shapes used in the GJK algorithm.
 * @details This class provides an interface for defining geometric shapes to be used in the GJK algorithm
 * for collision detection and distance calculation.
 */
class RRTPLANNER_LIB_EXPORT IShape
{
public:
    /**
     * @brief Default constructor
     */
    IShape() = default;

    /**
     * @brief Default copy constructor
     */
    IShape(const IShape& other) = default;

    /**
     * @brief Virtual destructor.
     */
    virtual ~IShape() = default;

    /**
     * @brief Create a clone of the shape.
     * @return A pointer to a new IShape object that is a copy of this shape.
     * @details This function creates a deep copy of the shape. The caller is responsible for managing the memory of the returned object.
     */
    virtual IShape* clone() const = 0;

    /**
     * @brief Get the centroid of the shape.
     * @return The centroid of the shape as a VectorF object.
     */
    virtual VectorF centroid() const = 0;

    /**
     * @brief Get the support point of the shape in a given direction.
     * @param dir The input search direction vector.
     * @return The support point of the shape in the given input direction as a VectorF object.
     */
    virtual VectorF support(const VectorF& dir) const = 0;

    /**
     * @brief Generate a debug string representation of the shape.
     * @return A QString containing a debug string representation of the shape.
     * @details The derived classes must override this function to provide their own debug representation.
     *          The debugPrint function is used in overloading the << operator for debugging purposes.
     */
    virtual QString debugPrint() const = 0;

    // Overloading the << operator
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, \
                                                   const RRTPLANNER_NAMESPACE::framework::algorithm::gjk::IShape& data)
    {
        QDebugStateSaver saver(debug);
        debug.nospace() << data.debugPrint();
        return debug;
    }

};

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
