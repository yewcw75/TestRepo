/**
 * @file Segment.h
 * Segment class to store segments of a plan.
 *
 * Each segment contains a WayptPrev and a WayptNext, from which a tangent vector along the segment, tVec, and also
 * a normal vector to the segment, nVec, can be determined.
 *
 * Each segment can also store the bisectors wrt to its preceding and following segments, bVecPrev and bVecNext respectively.
 *
 * @author: ycw
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
class RRTPLANNER_LIB_EXPORT Segment
{
public:
    Segment();
    Segment(const Waypt& wayptPrev, const Waypt& wayptNext, int id, bool isZeroLengthSegment = false);
    virtual ~Segment();
    Segment(const Segment& other);
    Segment& operator=(const Segment& other);


    void set(const Waypt& wayptPrev, const Waypt& wayptNext, int id, bool isZeroLengthSegment = false);

    void setId(int id);
    int id() const;

    void setIsZeroLengthSegment(bool isZeroLengthSegment);
    bool isZeroLengthSegment() const;

    void setWayptPrev(const Waypt& wayptPrev); //set previous waypt of segment.
    const Waypt& wayptPrev() const; //previous waypt of segment    
    void setWayptNext(const Waypt& wayptNext); //set next waypt of segment.
    const Waypt& wayptNext() const; //next waypt of segment

    void setTVec(const VectorF& tVec);  //[m][m] tangent unit vector fron wayptPrev to wayptNext
    const VectorF& tVec() const; //[m][m] tangent unit vector fron wayptPrev to wayptNext
    void setNVec(const VectorF& nVec);  //[m][m] unit vector normal to tVec. tVec x nVec = positive (down-dir)
    const VectorF& nVec() const; //[m][m] unit vector normal to tVec. tVec x nVec = positive (down-dir)

    void setLength(double length); //[m] Segment length
    double length() const; //[m] Segment length

    void setLengthCumulative(double lengthCumulative); //[m] cumulative length in a plan
    double lengthCumulative() const; //[m] cumulative length in a plan

    //set bisectors
    void setbVecPrev(const VectorF& bVecPrev);
    void setbVecPrev(const Segment& segmentPrev); //set bisector with a previous segment
    const VectorF& bVecPrev() const; //[m][m] bisector with previous segment.

    void setbVecNext(const VectorF& bVecNext);
    void setbVecNext(const Segment& segmentNext); //set bisector with a next segment.
    const VectorF& bVecNext() const; //[m][m] bisector with next segment

    //Set segment length, tVec and nVec.
    //If isZeroLengthSegment is true, only the segment length will be set.
    //Important: call this method only after wayptPrev and wayptNext had been set.
    void setSegmentAttributes();

private:
    QSharedDataPointer<SegmentPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
