/**
 * @file SimplexMinDist.h
 * @brief Definition of the SimplexMinDist class, a concrete implementation of Simplex class.
 * To used in GjkMinDist.
 *
 * @see GjkMinDist, Simplex, IShape
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_MINDIST_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_MINDIST_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SimplexMinDistPrivate;

/**
 * @brief Definition of the SimplexMinDist class, a concrete implementation of Simplex class.
 * To used in GjkMinDist.
 * @see GjkMinDist, Simplex, IShape
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST SimplexMinDist : public Simplex
{
public:
    /**
     * @brief Default constructor.
     */
    SimplexMinDist();

    /**
     * @brief Constructor with a specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a simplex
     *                   vertex/edge. If not set, the default value is 1e-6.
     */
    SimplexMinDist(double eps_square);

    /**
     * @brief Virtual destructor.
     */
    virtual ~SimplexMinDist();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The SimplexMinDist object to copy from.
     */
    SimplexMinDist(const SimplexMinDist& other) = delete;

    /**
     * @brief Reset the simplex.
     * @details This function clears the internal vertex list of the simplex.
     */
    virtual void reset() override;

    /**
     * @brief Update the simplex with a new support point.
     * @param vertex The new support point to add to the simplex.
     * @param[out] v The vector along the next search direction (un-normalized).
     * @return True if the origin is in the simplex, false otherwise.
     */
    [[nodiscard]] virtual bool update(const VectorF& vertex, VectorF& v) override;

private:
    QScopedPointer<SimplexMinDistPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
