/**
 * @file Simplex.h
 * @brief Abstract Simplex class, for the Gilbert-Johnson-Keerthi (GJK) algorithm.
 *
 *
 * @see Gjk, IShape
 * @authors Enric Xargay Mata, ycw
 * @date 2023-07-24
 */
#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QScopedPointer>


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SimplexPrivate;

/**
 * @brief The Simplex class represents an abstract simplex used in the GJK algorithm for collision detection.
 *
 * @see Gjk, IShape
 */
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST Simplex
{
public:
    /**
     * @brief Default constructor.
     */
    Simplex();

    /**
     * @brief Constructor with a specified threshold for comparing distances.
     * @param eps_square The square of the threshold to compare distances and decide if a point is on a simplex
     *                   vertex/edge. If not set, the default value is 1e-6.
     */
    Simplex(double eps_square);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Simplex();

    /**
     * @brief Copy constructor (deleted to make the class non-copyable).
     * @param other The Simplex object to copy from.
     */
    Simplex(const Simplex& other) = delete;

    /**
     * @brief Reset the simplex, clearing its internal vertex list.
     * @details This function clears the internal vertex list of the simplex, effectively resetting it to an
     * empty state.
     */
    virtual void reset() = 0;

    /**
     * @brief Update the simplex with a new support point.
     * @param vertex The new support point to add to the simplex.
     * @param[out] v The vector along the next search direction (un-normalized) used during the GJK algorithm.
     * @return True if the origin is in the simplex, false otherwise.
     */
    [[nodiscard]] virtual bool update(const VectorF& vertex, VectorF& v) = 0;

public:
    /**
     * @brief Set the threshold for comparing distances in the simplex.
     * @param eps_square The square of the threshold value to compare distances and decide if a point is on a
     *                   simplex vertex/edge. If not set, the default value is 1e-6.
     */
    void set_eps_square(double eps_square);

    /**
     * @brief Get the threshold value for comparing distances in the simplex.
     * @return The square of the threshold value for comparing distances.
     */
    double eps_square() const;

private:
    QScopedPointer<SimplexPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
