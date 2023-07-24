/**
 * @file VectorFHelper.h
 * @brief Helper class for VectorF.
 *
 * The VectorFHelper class provides helper functions that abstract away the underlying library operations for VectorF objects.
 * These functions make it easier to refactor the code if there is a need to change the underlying data structures in the future.
 *
 * @see VectorF.h
 * @authors Enric Xargay Mata, ycw
 */

#ifndef RRTPLANNER_LIB_VECTORFHELPER_H
#define RRTPLANNER_LIB_VECTORFHELPER_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

/**
 * @class VectorFHelper
 * @brief Helper class for VectorF.
 *
 * The VectorFHelper class provides helper functions that abstract away the underlying library operations for VectorF objects.
 */
class RRTPLANNER_LIB_EXPORT VectorFHelper
{
public:
    /**
     * @brief Default constructor.
     */
    VectorFHelper();

    /**
     * @brief Destructor.
     */
    ~VectorFHelper();

    /**
     * @brief Computes the Euclidean norm (L2 norm) of the given VectorF object.
     * @param vec The VectorF object for which the norm is computed.
     * @return The computed norm.
     */
    static double norm2(const VectorF& vec);

    /**
     * @brief Multiplies a VectorF object by a scalar value. scalar * vec.
     * @param vec The VectorF object to be multiplied.
     * @param scalar The scalar value.
     * @return The resulting VectorF object.
     */
    static VectorF multiply_value(const VectorF& vec, double scalar);

    /**
     * @brief Adds two VectorF objects. vec1 + vec2.
     * @param vec1 The first VectorF object.
     * @param vec2 The second VectorF object.
     * @return The resulting VectorF object.
     */
    static VectorF add_vector(const VectorF& vec1, const VectorF& vec2);

    /**
     * @brief Subtracts one VectorF object from another. vec1 - vec2.
     * @param vec1 The VectorF object from which the other VectorF object is subtracted.
     * @param vec2 The VectorF object to be subtracted.
     * @return The resulting VectorF object.
     */
    static VectorF subtract_vector(const VectorF& vec1, const VectorF& vec2);

    /**
     * @brief Computes the dot product of two VectorF objects. vec1 dot vec2.
     * @param vec1 The first VectorF object.
     * @param vec2 The second VectorF object.
     * @return The computed dot product.
     */
    static double dot_product(const VectorF& vec1, const VectorF& vec2);

    /**
     * @brief Computes the z value of the cross product between two 2d VectorF objects. vec1 cross vec2.
     * @param vec1 The first VectorF object.
     * @param vec2 The second VectorF object.
     * @return z value of the computed cross product.
     * @note qWarning if vec1 and vec2 are not of dim 2
     */
    static double cross_product_zVal(const VectorF& vec1, const VectorF& vec2);

    /**
     * @brief Computes the cross product between two 3d VectorF objects. vec1 cross vec2.
     * @param vec1 The first VectorF object.
     * @param vec2 The second VectorF object.
     * @return The computed cross product.
     * @note qWarning if vec1 and vec2 are not of dim 3
     */
    static VectorF cross_product(const VectorF& vec1, const VectorF& vec2);

    /**
     * @brief First pads the input 2d vector (with the two dimensions being x and y respectively) with 0 in the z-dimension.
     * Then the cross product with z-vector = [0; 0; 1] is computed. Note that the z-dimension of the cross product is expected to be zero and
     * it will be trimmed before being returned.
     * @param vec The input VectorF object.
     * @return The computed cross product with the z-vector. Dimension = 2.
     * @note qWarning if vec is not of dim 2
     */
    static VectorF vec2D_cross_z(const VectorF& vec);

    /**
     * @brief compare Compares two VectorF objects and return true if norm_2 of their difference is smaller than the specified tolerance.
     * @param d1 first VectorF input to compare
     * @param d2 second VectorF input to compare
     * @param tol_small tolerance level
     * @return bool True => norm_2 of the difference between d1 and d2 are smaller or equal to tol_small.
     */
    static bool compare(const VectorF& vec1, const VectorF& vec2, double tol_small);
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif

