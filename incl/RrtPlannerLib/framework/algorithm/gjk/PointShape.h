/**
 * @file PointShape.h
 * @brief Definition of the PointShape class, representing a point shape for use in the GJK algorithm.
 *
 * This header file defines the PointShape class, which is derived from the IShape interface. The PointShape class
 * represents a point and provides methods for computing its centroid, support point, and other
 * essential properties. It is designed to be used in the Gilbert-Johnson-Keerthi (GJK) algorithm for collision
 * detection and distance calculation between geometric shapes.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_POINTSHAPE_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_POINTSHAPE_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

/**
 * @brief The PointShape class represents a point
 * @details This class inherits from the IShape interface, allowing it to be used with the GJK (Gilbert–Johnson–Keerthi) algorithm.
 */
class PointShapePrivate; // Forward declaration for the private data class.
class PointShape : public IShape
{
public:
    /**
     * @brief Default constructor to create a PointShape object.
     */
    PointShape();

    /**
     * @brief Constructor.
     */
    PointShape(const VectorF& pt);

    /**
     * @brief Copy constructor to create a PointShape object as a copy of another PointShape.
     * @param other The PointShape object to copy from.
     */
    PointShape(const PointShape& other);

    /**
     * @brief Copy assignment operator to copy the content of another PointShape object to this object.
     * @param other The PointShape object to copy from.
     * @return A reference to this PointShape object after the copy assignment.
     */
    PointShape& operator=(const PointShape& other);

    /**
     * @brief Destructor to destroy the PointShape object.
     */
    virtual ~PointShape();

    /**
     * @brief Get the point of the shape.
     * @return A const reference to the point as a VectorF object.
     */
    const VectorF& pt() const;

    /**
     * @brief Set the point of the shape.
     * @param pt The new point to set as a VectorF object.
     */
    void setPt(const VectorF& pt);

    /**
     * @brief Create a clone of the shape.
     * @return A pointer to a new IShape object that is a copy of this shape.
     * @details This function creates a deep copy of the shape. The caller is responsible for managing the memory of the returned object.
     */
    virtual IShape* clone() const override;

    /**
     * @brief Get the centroid of the shape.
     * @return The centroid of the shape as a VectorF object.
     */
    virtual VectorF centroid() const override;

    /**
     * @brief Get the support point of the shape in a given direction.
     * @param dir The input search direction vector.
     * @return The support point of the shape in the given input direction as a VectorF object.
     */
    virtual VectorF support(const VectorF& dir) const override;

protected:
    /**
     * @brief Generate a debug string representation of the shape.
     * @return A QString containing a debug string representation of the shape.
     * @details The derived classes must override this function to provide their own debug representation.
     *          The debugPrint function is used in overloading the << operator for debugging purposes.
     */
    virtual QString debugPrint() const override;

    /**
     * @brief Detach this class's private QSharedDataPointer during cloning.
     */
    virtual void detach();

private:
    QSharedDataPointer<PointShapePrivate> d_ptr; // Private data pointer for Pimpl idiom.
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
