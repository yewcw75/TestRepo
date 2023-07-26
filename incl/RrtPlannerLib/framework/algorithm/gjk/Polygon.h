/**
 * @file Polygon.h
 * @brief Definition of the Polygon class, representing a polygon shape for use in the GJK algorithm.
 *
 * This header file defines the Polygon class, which is derived from the IShape interface. The Polygon class
 * represents a 2D polygon shape and provides methods for computing its centroid, support point, and other
 * essential properties. It is designed to be used in the Gilbert-Johnson-Keerthi (GJK) algorithm for collision
 * detection and distance calculation between geometric shapes.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */

#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_POLYGON_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_POLYGON_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QString>
#include <QVector>
#include <QSharedDataPointer>

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class PolygonPrivate;

/**
 * @brief The Polygon class represents a polygon shape.
 * @details This class is derived from the IShape interface and provides an implementation for a polygon shape
 * in the Gilbert-Johnson-Keerthi (GJK) algorithm for collision detection and distance calculation.
 */
class RRTPLANNER_LIB_EXPORT Polygon : public IShape
{
public:
    /**
     * @brief Default constructor.
     */
    Polygon();

    /**
     * @brief Constructor with an initializer list of vertices.
     * @param list An initializer list of VectorF representing the vertices of the polygon.
     */
    Polygon(const std::initializer_list<VectorF>& list);

    /**
     * @brief Copy constructor.
     * @param other The Polygon object to copy from.
     */
    Polygon(const Polygon& other);

    /**
     * @brief Assignment operator.
     * @param other The Polygon object to assign from.
     * @return A reference to this Polygon object after the assignment.
     */
    Polygon& operator=(const Polygon& other);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Polygon();

    /**
     * @brief Create a deep copy of the Polygon object.
     * @return A pointer to a new Polygon object that is a copy of this Polygon.
     * @details The caller is responsible for managing the memory of the returned object.
     */
    virtual Polygon* clone() const override;

    /**
     * @brief Get the centroid of the Polygon.
     * @return The centroid of the Polygon as a VectorF object.
     */
    virtual VectorF centroid() const override;

    /**
     * @brief Get the support point of the Polygon in a given direction.
     * @param dir The input direction vector.
     * @return The support point of the Polygon in the given input direction as a VectorF object.
     * @details The input direction vector (dir) does not need to be a unit vector.
     */
    virtual VectorF support(const VectorF& dir) const override;

    /**
     * @brief Get the number of vertices in the Polygon.
     * @return The number of vertices in the Polygon.
     */
    int size() const;

    /**
     * @brief Get the vertex at a specific index.
     * @param i The index of the vertex to retrieve.
     * @return A constant reference to the VectorF representing the vertex at the specified index.
     */
    const VectorF& at(int i) const;

    /**
     * @brief Get the vertex at a specific index for modification.
     * @param i The index of the vertex to retrieve.
     * @return A reference to the VectorF representing the vertex at the specified index.
     */
    VectorF& operator[](int i);

    /**
     * @brief Get a constant reference to the list of vertices of the Polygon.
     * @return A constant reference to the QVector of VectorF representing the vertices of the Polygon.
     */
    const QVector<VectorF>& vertexList_const_ref() const;

    /**
     * @brief Get a reference to the list of vertices of the Polygon for modification.
     * @return A reference to the QVector of VectorF representing the vertices of the Polygon.
     */
    QVector<VectorF>& vertexList();

protected:
    /**
     * @brief Generate a debug string representation of the Polygon.
     * @return A QString containing a debug string representation of the Polygon.
     * @details The derived classes must override this function to provide their own debug representation.
     *          The debugPrint function is used in overloading the << operator for debugging purposes.
     */
    virtual QString debugPrint() const override;

    /**
     * @brief detach Detach this class's private QSharedDataPointer during cloning.
     */
    virtual void detach();

private:
    QSharedDataPointer<PolygonPrivate> d_ptr;
};

/**
 * @brief The namespace for the Gjk algorithm in the RRTPlanner framework.
 */
RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
