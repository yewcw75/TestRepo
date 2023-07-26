/**
 * @file Waypt.h
 * Waypt class to store waypoints of a plan.
 *
 * This file contains the declaration of the Waypt class, which represents a waypoint in a plan.
 * It provides functionalities for setting and accessing various properties of a waypoint, such as northing, easting, ID, and reference longitude.
 * @authors Enric Xargay Mata, ycw
 * @date 2023-06-30
 */
#ifndef RRTPLANNER_LIB_WAYPT_H
#define RRTPLANNER_LIB_WAYPT_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class WayptPrivate;
/**
 * @class Waypt
 * @brief The Waypt class represents a waypoint in a plan.
 */
class RRTPLANNER_LIB_EXPORT Waypt
{
public:
    /**
     * @brief Default constructor.
     */
    Waypt();

    /**
     * @brief Constructor overload.
     * @param northing_m The northing value in meters.
     * @param easting_m The easting value in meters.
     * @param lon0_deg The reference longitude in degrees.
     * @param id The waypt id.
     */
    Waypt(double northing_m, double easting_m, double lon0_deg = 0.0, int id = -1);

    /**
     * @brief Constructor overload.
     * @param coord The coordinate vector containing the northing and easting values in meters.
     * @param lon0_deg The reference longitude in degrees.
     * @param id The waypt id.
     */
    Waypt(const VectorF& coord, double lon0_deg = 0.0, int id = -1);

    /**
     * @brief Destructor.
     */
    virtual ~Waypt();

    /**
     * @brief Copy constructor.
     */
    Waypt(const Waypt& other);

    /**
     * @brief Assignment operator.
     */
    Waypt& operator=(const Waypt& other);

    /**
     * @brief Sets the waypoint with the given northing, easting, reference longitude, and ID.
     * @param northing_m The northing value in meters.
     * @param easting_m The easting value in meters.
     * @param lon0_deg The reference longitude in degrees. Default = 0.0.
     * @param id The waypt id. Default = -1.
     */
    void set(double northing_m, double easting_m, double lon0_deg = 0.0, int id = -1);

    /**
     * @brief Sets the waypoint with the given coordinate vector, reference longitude, and ID.
     * @param coord The coordinate vector containing the northing and easting values in meters.
     * @param lon0_deg The reference longitude in degrees. Default = 0.0.
     * @param id The waypt id. Default = -1.
     */
    void set(const VectorF& coord, double lon0_deg = 0.0, int id = -1);

    /**
     * @brief Sets the northing value of the waypoint.
     * @param northing_m The northing value in meters.
     */
    void setNorthing(double northing_m);

    /**
     * @brief Gets the northing value of the waypoint.
     * @return The northing value in meters.
     */
    double northing() const;

    /**
     * @brief Sets the easting value of the waypoint.
     * @param easting_m The easting value in meters.
     */
    void setEasting(double easting_m);

    /**
     * @brief Gets the easting value of the waypoint.
     * @return The easting value in meters.
     */
    double easting() const;

    /**
     * @brief Sets the waypt id.
     * @param id The waypt id.
     */
    void setId(int id);

    /**
     * @brief Gets the waypt id.
     * @return The waypt id.
     */
    int id() const;

    /**
     * @brief Sets the reference longitude of the waypoint.
     * @param lon0_deg The reference longitude in degrees.
     */
    void setLon0(double lon0_deg);

    /**
     * @brief Gets the reference longitude of the waypoint.
     * @return The reference longitude in degrees.
     */
    double lon0_deg() const;

    /**
     * @brief Sets the coordinate of the waypoint using the given coordinate vector.
     * @param coord The coordinate vector containing the northing and easting values.
     */
    void setCoord(const VectorF& coord);

    /**
     * @brief Gets a constant reference to the coordinate vector of the waypoint.
     * @return The constant reference to the coordinate vector.
     */
    const VectorF& coord_const_ref() const;

    // Overloading the << operator
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::Waypt &data);

private:
    QSharedDataPointer<WayptPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
