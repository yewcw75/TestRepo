/**
 * @file Segment.h
 * @brief Segment class to store a segment of a plan.
 *
 * The Segment class represents a segment of a plan and contains information about the previous and next waypoints,
 * segment attributes such as length, unit tangent vector, and unit normal vector, as well as bisectors with respect
 * to its preceding and following segments.
 *
 * Once the previous and next waypoints are set, the setSegmentAttributes() function can be called to calculate the
 * segment length, the unit tangent vector along the segment (tVec), and the unit normal vector to the segment (nVec).
 *
 * @author ycw
 * @date 20230630
 */

#ifndef RRTPLANNER_LIB_SEGMENT_H
#define RRTPLANNER_LIB_SEGMENT_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <QSharedDataPointer>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SegmentPrivate;

/**
 * @class Segment
 * @brief Segment class to store segments of a plan.
 *
 * The Segment class represents a segment of a plan and contains information about the previous and next waypoints,
 * segment attributes such as length, unit tangent vector, and unit normal vector, as well as bisectors with respect
 * to its preceding and following segments.
 */
class RRTPLANNER_LIB_EXPORT Segment
{
public:
    /**
     * @brief Default constructor.
     */
    Segment();

    /**
     * @brief Constructor that initializes the segment with the given previous and next waypoints and ID.
     * @param wayptPrev The previous waypoint of the segment.
     * @param wayptNext The next waypoint of the segment.
     * @param id The ID of the segment.
     */
    Segment(const Waypt& wayptPrev, const Waypt& wayptNext, int id);

    /**
     * @brief Destructor.
     */
    virtual ~Segment();

    /**
     * @brief Copy constructor.
     */
    Segment(const Segment& other);

    /**
     * @brief Assignment operator.
     */
    Segment& operator=(const Segment& other);

    /**
     * @brief Sets the previous and next waypoints of the segment.
     * @param wayptPrev The previous waypoint of the segment.
     * @param wayptNext The next waypoint of the segment.
     * @param id The ID of the segment.
     */
    void set(const Waypt& wayptPrev, const Waypt& wayptNext, int id);

    /**
     * @brief Sets the ID of the segment.
     * @param id The ID of the segment.
     */
    void setId(int id);

    /**
     * @brief Gets the ID of the segment.
     * @return The ID of the segment.
     */
    int id() const;

    /**
     * @brief Sets the previous waypoint of the segment.
     * @param wayptPrev The previous waypoint of the segment.
     */
    void setWayptPrev(const Waypt& wayptPrev);

    /**
     * @brief Gets the previous waypoint of the segment.
     * @return The previous waypoint of the segment.
     */
    const Waypt& wayptPrev() const;

    /**
     * @brief Sets the next waypoint of the segment.
     * @param wayptNext The next waypoint of the segment.
     */
    void setWayptNext(const Waypt& wayptNext);

    /**
     * @brief Gets the next waypoint of the segment.
     * @return The next waypoint of the segment.
     */
    const Waypt& wayptNext() const;

    /**
     * @brief Sets the unit tangent vector along the segment.
     * @param tVec The unit tangent vector.
     */
    void setTVec(const VectorF& tVec);

    /**
     * @brief Gets the unit tangent vector along the segment.
     * @return The unit tangent vector.
     */
    const VectorF& tVec() const;

    /**
     * @brief Sets the unit normal vector to the segment.
     * @param nVec The unit normal vector.
     */
    void setNVec(const VectorF& nVec);

    /**
     * @brief Gets the unit normal vector to the segment.
     * @return The unit normal vector.
     */
    const VectorF& nVec() const;

    /**
     * @brief Sets the length of the segment.
     * @param length The length of the segment.
     */
    void setLength(double length);

    /**
     * @brief Gets the length of the segment.
     * @return The length of the segment.
     */
    double length() const;

    /**
     * @brief Sets the cumulative length of the segment in a plan.
     * @param lengthCumulative The cumulative length of the segment.
     */
    void setLengthCumulative(double lengthCumulative);

    /**
     * @brief Gets the cumulative length of the segment in a plan.
     * @return The cumulative length of the segment.
     */
    double lengthCumulative() const;

    /**
     * @brief Sets the bisector vector with respect to the previous segment.
     * @param bVecPrev The bisector vector.
     */
    void setbVecPrev(const VectorF& bVecPrev);

    /**
     * @brief Sets the bisector vector with respect to the previous segment.
     * @param segmentPrev The previous segment.
     */
    void setbVecPrev(const Segment& segmentPrev);

    /**
     * @brief Gets the bisector vector with respect to the previous segment.
     * @return The bisector vector.
     */
    const VectorF& bVecPrev() const;

    /**
     * @brief Sets the bisector vector with respect to the next segment.
     * @param bVecNext The bisector vector.
     */
    void setbVecNext(const VectorF& bVecNext);

    /**
     * @brief Sets the bisector vector with respect to the next segment.
     * @param segmentNext The next segment.
     */
    void setbVecNext(const Segment& segmentNext);

    /**
     * @brief Gets the bisector vector with respect to the next segment.
     * @return The bisector vector.
     */
    const VectorF& bVecNext() const;

    /**
     * @brief Calculates and sets the segment attributes (length, tVec, nVec).
     */
    void setSegmentAttributes();

    /**
     * @brief Overloaded output stream insertion operator.
     * @param debug The debug stream.
     * @param data The Segment object to be inserted into the stream.
     * @return The debug stream with the Segment object inserted.
     */
    friend RRTPLANNER_LIB_EXPORT QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::Segment& data);

private:
    QSharedDataPointer<SegmentPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
