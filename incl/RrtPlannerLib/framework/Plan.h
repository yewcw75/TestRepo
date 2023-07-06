/**
 * @file Plan.h
 * @brief Defines the Plan class, which stores a list of segments and additional properties.
 *
 * The Plan class provides functionality to store a list of segments and manage properties such as whether it is a nominal plan or a limit plan.
 *
 * @authors Enric Xargay Mata, ycw
 * @date 20230630
 */

#ifndef RRTPLANNER_LIB_PLAN_H
#define RRTPLANNER_LIB_PLAN_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/Segment.h>
#include <QSharedDataPointer>
#include <QObject>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class PlanPrivate;
class RRTPLANNER_LIB_EXPORT Plan
{
    friend class PlanHelper;
    Q_GADGET
public:
    /**
     * @enum Property
     * @brief Represents different properties of a Plan.
     */
    enum class Property{
        NONE            = 0,            ///< No property.
        IS_NOMINAL      = 1 << 0,       ///< Indicates if the plan is a nominal plan.
        IS_LIMIT        = 1 << 1,       ///< Indicates if the plan is a limit plan.
    };
    Q_FLAG(Property) // Q_ENUM is also called in Q_FLAG()
    Q_DECLARE_FLAGS(PropertyFlags, Property)

public:
    /**
     * @brief Default constructor.
     */
    Plan();

    /**
     * @brief Destructor.
     */
    virtual ~Plan();

    /**
     * @brief Copy constructor.
     */
    Plan(const Plan& other);

    /**
     * @brief Assignment operator.
     */
    Plan& operator=(const Plan& other);

    /**
     * @brief Clears the plan, resetting all parameters and property flags.
     */
    void clearPlan();

    /**
     * @brief Sets the plan using the given waypoint list and segment ID list.
     * @param wayptList The list of waypoints to set the plan.
     * @param segIdList The list of segment IDs to assign. The size of the vector should be (wayptList.size() - 1).
     * @param resultsDesc Optional pointer to return the description of the result.
     * @return True if the plan is successfully set, false otherwise.
     */
    bool setPlan(const QVector<Waypt>& wayptList,
                 const QVector<int>& segIdList = QVector<int>(),
                 QString* resultsDesc = nullptr);

    /**
     * @brief Sets the plan using the given waypoint list, segment ID list, and plan ID.
     * @param wayptList The list of waypoints to set the plan.
     * @param segIdList The list of segment IDs to assign. The size of the vector should be (wayptList.size() - 1).
     * @param id The plan ID.
     * @param resultsDesc Optional pointer to return the description of the result.
     * @return True if the plan is successfully set, false otherwise.
     */
    bool setPlan(const QVector<Waypt>& wayptList,
                 const QVector<int>& segIdList,
                 int id,
                 QString* resultsDesc = nullptr);

    /**
     * @brief Sets the plan using the given waypoint list and plan ID.
     * @param wayptList The list of waypoints to set the plan.
     * @param id The plan ID.
     * @param resultsDesc Optional pointer to return the description of the result.
     * @return True if the plan is successfully set, false otherwise.
     *
     * The segment IDs will be automatically assigned from [0, wayptList.size() - 1].
     */
    bool setPlan(const QVector<Waypt>& wayptList,
                 int id,
                 QString* resultsDesc = nullptr);

    /**
     * @brief Sets the ID of the plan.
     * @param id The plan ID to set.
     */
    void setId(int id);

    /**
     * @brief Gets the ID of the plan.
     * @return The plan ID.
     */
    int id() const;

    /**
     * @brief Gets the number of segments in the plan.
     * @return The number of segments.
     */
    int nSegment() const;

    /**
     * @brief Gets the number of waypoints in the plan.
     * @return The number of waypoints.
     */
    int nWaypt() const;

    /**
     * @brief Gets the list of waypoints in the plan.
     * @return The list of waypoints.
     */
    QVector<Waypt> wayptList() const;

    /**
     * @brief Gets the list of segments in the plan.
     * @return The list of segments.
     */
    const QVector<Segment>& segmentList() const;

    /**
     * @brief Gets the total length of the plan.
     * @return The total length of the plan in meters.
     */
    double length() const;

    /**
     * @brief Sets the cross-track distance to the nominal plan.
     * @param crossTrack The cross-track distance in meters.
     */
    void setCrossTrack(double crossTrack);

    /**
     * @brief Gets the cross-track distance to the nominal plan.
     * @return The cross-track distance in meters.
     */
    double crossTrack() const;

    /**
     * @brief Sets the specified property flag to the given state.
     * @param property The property flag to set.
     * @param on The state of the property flag (true for on, false for off).
     */
    void setProperty(const Property& property, bool on = true);

    /**
     * @brief Tests if the specified property flag is set.
     * @param property The property flag to test.
     * @return True if the property flag is set, false otherwise.
     */
    bool testProperty(const Property& property) const;

    /**
     * @brief Sets the property flags for the plan.
     * @param flags The property flags to set.
     */
    void setPropertyFlags(const PropertyFlags& flags);

    /**
     * @brief Gets the property flags of the plan.
     * @return The property flags.
     */
    const PropertyFlags& propertyFlags() const;

    /**
     * @brief Overloads the << operator to output the Plan object to the debug stream.
     * @param debug The debug stream.
     * @param data The Plan object to output.
     * @return The debug stream with the Plan object.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::Plan &data);

protected:
    /**
     * @brief Appends a segment to the plan.
     * @param segment The segment to append.
     *
     * Note: The appended segment's cumulative length will be set by adding its length with previous cumulative length.
     * Make sure the segment length of the input segment has been set correctly.
     */
    void appendSegment(const Segment& segment);

    /**
     * @brief Sets the list of segments for the plan.
     * @param segmentList The list of segments to set.
     */
    void setSegmentList(const QVector<Segment>& segmentList);

private:
    QSharedDataPointer<PlanPrivate> mp_pimpl;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Plan::PropertyFlags)

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif // RRTPLANNER_LIB_PLAN_H
