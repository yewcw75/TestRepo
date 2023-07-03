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
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/Waypt.h>
#include <QSharedDataPointer>
#include <QObject>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SegmentPrivate;
class RRTPLANNER_LIB_EXPORT Segment
{
    friend class PlanHelper;
    Q_GADGET //needed for Q_FLAG macro

public:
    enum class Field{
        NONE        = 0,
        ID          = 1 << 0,
        WAYPT_PREV  = 1 << 1,
        WAYPT_NEXT  = 1 << 2,
        TVEC        = 1 << 3,
        NVEC        = 1 << 4,
        LENGTH      = 1 << 5,
        BVEC_PREV   = 1 << 6,
        BVEC_NEXT   = 1 << 7
    };
    Q_FLAG(Field) //Q_ENUM is also called in Q_FLAG()
    Q_DECLARE_FLAGS(FieldFlags, Field) //FieldFlags to keep track of fields that had been set

public:
    Segment();
    Segment(const Waypt& wayptPrev, const Waypt& wayptNext, int id, bool isZeroLengthSegment = false);
    virtual ~Segment();
    Segment(const Segment& other);
    Segment& operator=(const Segment& other);

    void set(const Waypt& wayptPrev, const Waypt& wayptNext, int id, bool isZeroLengthSegment = false);

    void setId(int id);
    int id() const;
    void setWayptPrev(const Waypt& wayptPrev); //previous waypt of segment
    const Waypt& wayptPrev() const; //previous waypt of segment
    void setWayptNext(const Waypt& wayptNext); //next waypt of segment
    const Waypt& wayptNext() const; //next waypt of segment

    //Note: Both tVec and nVec will be automatically calculated upon setting of both wayptPrev and wayptNext.
    const VectorF& tVec() const; //[m][m] tangent unit vector fron wayptPrev to wayptNext
    const VectorF& nVec() const; //[m][m] unit vector normal to tVec. tVec x nVec = positive (down-dir)
    float length() const; //[m]

    //set bisectors
    void setbVecPrev(const VectorF& bVecPrev);
    void setbVecPrev(const Segment& segmentPrev); //set bisector with a previous segment
    const VectorF& bVecPrev() const; //[m][m] bisector with previous segment.

    void setbVecNext(const VectorF& bVecNext);
    void setbVecNext(const Segment& segmentNext); //set bisector with a next segment.
    const VectorF& bVecNext() const; //[m][m] bisector with next segment

    //presence flags of fields
    const FieldFlags& getFieldFlags() const;

protected:
    void setTVec(const VectorF& tVec);  //[m][m] tangent unit vector fron wayptPrev to wayptNext
    void setNVec(const VectorF& nVec);  //[m][m] unit vector normal to tVec. tVec x nVec = positive (down-dir)

private:
    QSharedDataPointer<SegmentPrivate> mp_pimpl;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Segment::FieldFlags)

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
