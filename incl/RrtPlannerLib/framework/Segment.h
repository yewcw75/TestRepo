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
    Q_GADGET //needed for Q_FLAG macro

public:
    enum class Field{
        NONE      = 0,
        ID        = 1 << 0,
        WAYPT_PREV = 1 << 1,
        WAYPT_NEXT = 1 << 2,
        TVEC      = 1 << 3,
        NVEC      = 1 << 4,
        LENGTH    = 1 << 5,
        BVEC_PREV = 1 << 6,
        BVEC_NEXT = 1 << 7
    };
    Q_FLAG(Field) //Q_ENUM is also called in Q_FLAG()
    Q_DECLARE_FLAGS(FieldFlags, Field) //FieldFlags to indicate fields that had been set

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
    Waypt wayptPrev() const; //previous waypt of segment
    void setWayptNext(const Waypt& wayptNext); //next waypt of segment
    Waypt wayptNext() const; //next waypt of segment

    //automatically calculated once both wayptPrev and wayptNext are set
    Vector_NE tVec() const; //[m][m] tangent unit vector fron wayptPrev to wayptNext
    Vector_NE nVec() const; //[m][m] unit vector normal to tVec. tVec x nVec = positive (down-dir)
    float length() const; //[m]

    //set bisectors
    void setbVecPrev(const Vector_NE& bVecPrev);
    void setbVecPrev(const Segment& segmentPrev); //set bisector with a previous segment
    Vector_NE bVecPrev() const; //[m][m] bisector with previous segment.

    void setbVecNext(const Vector_NE& bVecNext);
    void setbVecNext(const Segment& segmentNext); //set bisector with a next segment.
    Vector_NE bVecNext() const; //[m][m] bisector with next segment

    //presence flags of fields
    FieldFlags getFieldFlags() const;

private:
    QSharedDataPointer<SegmentPrivate> mp_pimpl;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Segment::FieldFlags)

RRTPLANNER_FRAMEWORK_END_NAMESPACE

#endif
