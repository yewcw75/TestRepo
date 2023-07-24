/**
 * @file Support.h
 * @brief Definition of an abstract Support class, which providing support point calculations for GJK algorithm.
 *
 * The Support class is designed to find the support point in the Minkowski difference
 * of two shapes in a given search direction.
 *
 * @see Gjk, IShape
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SUPPORT_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SUPPORT_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QScopedPointer>
#include <QObject> //for Q_GADGET

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class IShape;
class SupportPrivate;

/**
 * @brief Definition of an abstract Support class, which providing support point calculations for GJK algorithm.
 *
 * The Support class is designed to find the support point in the Minkowski difference
 * of two shapes in a given search direction.
 *
 * @see Gjk, IShape
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST Support
{
    Q_GADGET //for Q_ENUM
public:
    /**
     * @brief Enum class defining result flags for the support point calculation.
     */
    enum class ResultFlag {
        SUPPORT_ON_SIMPLEX,      /**< Support point is on the simplex. */
        SUPPORT_SHORT_OF_ORIGIN, /**< Support point is short of the origin. */
        SUPPORT_ON_ORIGIN,       /**< Support point is on the origin. */
        SUPPORT_BEYOND_ORIGIN    /**< Support point is beyond the simplex. */
    };
    Q_ENUM(ResultFlag);

public:
    /**
     * @brief Default constructor.
     */
    Support();

    /**
     * @brief Constructor with a specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a Support
     *                   vertex/edge. If not set, the default value is 1e-6.
     */
    Support(double eps_square);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Support();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The Support object to copy from.
     */
    Support(const Support& other) = delete;

    /**
     * @brief Reset the support object.
     * @details This function clears any internal data in the support object, effectively resetting it for subsequent
     * support point calculations.
     */
    virtual void reset() = 0;

    /**
     * @brief Compute the support point in a given search direction for the Minkowski difference of two shapes.
     * @param shape1 Pointer to the first shape.
     * @param shape2 Pointer to the second shape.
     * @param v The search direction vector.
     * @param isDirFromSupport2Origin True if the search direction, v, is in the direction from any support point of
     *                                 the Minkowski difference to the origin.
     * @param[out] spp The computed support point of the Minkowski difference in the search direction.
     * @return The result flag indicating the relationship of the support point with respect to the simplex and origin.
     */
    virtual Support::ResultFlag support(const IShape* shape1, const IShape* shape2,
                                         const VectorF& v, bool isDirFromSupport2Origin,
                                         VectorF& spp) = 0;

public:
    /**
     * @brief Set the square threshold for comparing distances in the support calculations.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a Support
     *                   vertex/edge. If not set, the default value is 1e-6.
     */
    void set_eps_square(double eps_square); //if not set, default = 1e-6

    /**
     * @brief Get the square threshold used in the support calculations.
     * @return The square of the threshold used to compare distances and decide if a point is on a Support vertex/edge.
     */
    double eps_square() const;

private:
    QScopedPointer<SupportPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
