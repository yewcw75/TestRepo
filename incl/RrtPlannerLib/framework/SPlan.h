/**
 * @file SPlan.h
 * @brief This file contains the declaration of the SPlan class.
 * @authors Enric Xargay Mata, ycw
 * @date 2023-08-08
 */

#ifndef RRTPLANNER_LIB_S_PLAN_H
#define RRTPLANNER_LIB_S_PLAN_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

/**
 * @class SPlan
 * @brief The SPlan class represents a plan with specific attributes.
 */
class SPlanPrivate;
class RRTPLANNER_LIB_EXPORT SPlan
{
public:
    /**
     * @brief Default constructor.
     */
    SPlan();

    /**
     * @brief Constructor with attributes.
     * @param crossTrack The cross-track value.
     * @param Lh The arc length horizon.
     * @param vol_cum The cumulative volume value.
     * @param area_cum The cumulative area value.
     */
    SPlan(double crossTrack, double Lh, double vol_cum, double area_cum);

    /**
     * @brief Destructor.
     */
    virtual ~SPlan();

    /**
     * @brief Copy constructor.
     */
    SPlan(const SPlan& other);

    /**
     * @brief Assignment operator.
     */
    SPlan& operator=(const SPlan& other);

    /**
     * @brief Get the cross-track value.
     * @return The cross-track value.
     */
    double getCrosstrack() const;

    /**
     * @brief Set the cross-track value.
     * @param crosstrack The cross-track value.
     */
    void setCrosstrack(double crosstrack);

    /**
     * @brief Get the arc length horizon value.
     * @return The arc length horizon value.
     */
    double getLh() const;

    /**
     * @brief Set the arc length horizon value.
     * @param lh The arc length horizon value.
     */
    void setLh(double lh);

    /**
     * @brief Get the cumulative volume value.
     * @return The cumulative volume value.
     */
    double getVol_cum() const;

    /**
     * @brief Set the cumulative volume value.
     * @param vol_cum The cumulative volume value.
     */
    void setVol_cum(double vol_cum);

    /**
     * @brief Get the cumulative area value.
     * @return The cumulative area value.
     */
    double getArea_cum() const;

    /**
     * @brief Set the cumulative area value.
     * @param area_cum The cumulative area value.
     */
    void setArea_cum(double area_cum);

    /**
     * @brief Overload of the << operator to output the SPlan object to the debug stream.
     * @param debug The debug stream.
     * @param data The SMap object to output.
     * @return The debug stream with the SPlan object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::SPlan &data);

private:
    QSharedDataPointer<SPlanPrivate> d_ptr;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif // RRTPLANNER_LIB_S_PLAN_H
