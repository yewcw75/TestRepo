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
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE
#endif

