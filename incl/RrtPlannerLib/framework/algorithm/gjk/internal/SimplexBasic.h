/**
 * @file SimplexBasic.h
 * @brief Definition of the SimplexBasic class, a concrete implementation for Simplex class.
 * To used in GjkBasic for basic GJK algorithm.
 *
 * @see GjkBasic, Simplex, IShape
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_BASIC_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_BASIC_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SimplexBasicPrivate;

/**
 * @brief Definition of the SimplexBasic class, a concrete implementation for Simplex class.
 * To used in GjkBasic for basic GJK algorithm.
 *
 * @see GjkBasic, Simplex, IShape
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST SimplexBasic : public Simplex
{
public:
    /**
     * @brief Default constructor.
     */
    SimplexBasic();

    /**
     * @brief Constructor with a specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a simplex
     *                   vertex/edge. If not set, the default value is 1e-6.
     */
    SimplexBasic(double eps_square);

    /**
     * @brief Virtual destructor.
     */
    virtual ~SimplexBasic();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The SimplexBasic object to copy from.
     */
    SimplexBasic(const SimplexBasic& other) = delete;

    /**
     * @brief Reset the simplex used in the basic GJK algorithm.
     * @details This function clears the internal vertex list of the simplex for basic GJK calculations.
     */
    virtual void reset() override;

    /**
     * @brief Update the simplex with a new support point for basic GJK calculations.
     * @param vertex The new support point to add to the simplex.
     * @param[out] v The vector along the next search direction (un-normalized).
     * @return True if the origin is in the simplex, false otherwise.
     */
    [[nodiscard]] virtual bool update(const VectorF& vertex, VectorF& v) override;

private:
    QScopedPointer<SimplexBasicPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
