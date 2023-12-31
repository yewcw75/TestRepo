#include <RrtPlannerLib/framework/Segment.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/UtilHelper.h>
#include <boost/geometry.hpp>
#include <QSharedData>
#include <QtGlobal>
#include <QDebug>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class SegmentPrivate: public QSharedData
{
public:
    SegmentPrivate()
        :QSharedData()
    {
        m_tVec.resize(DIM_COORD);
        m_nVec.resize(DIM_COORD);
        m_bVecPrev.resize(DIM_COORD);
        m_bVecNext.resize(DIM_COORD);
    }
    ~SegmentPrivate() = default;
    SegmentPrivate(const SegmentPrivate& other) = default;
    void calculateBisector(const Segment& seg1, const Segment& seg2, VectorF& bVec);

public:
    Waypt m_wayptPrev;
    Waypt m_wayptNext;
    VectorF m_tVec;
    VectorF m_nVec;
    VectorF m_bVecPrev;
    VectorF m_bVecNext;
    double m_length{};
    double m_lengthCumulative{};
    int m_id{-1};
};

//----------
void SegmentPrivate::calculateBisector(const Segment& seg1, const Segment& seg2, VectorF& bVec)
{
    VectorF v = VectorFHelper::add_vector(seg1.nVec(), seg2.nVec());
    double length_v = VectorFHelper::norm2(v);
    if(length_v > TOL_SMALL){
        bVec = VectorFHelper::multiply_value(v, 1/length_v);
    }
    else{
        //left-hand perp vector of b (tvec)
        bVec[IDX_NORTHING] = seg2.nVec()[IDX_EASTING];
        bVec[IDX_EASTING] = -seg2.nVec()[IDX_NORTHING];
    }
}

//##############################
//----------
Segment::Segment()
    :d_ptr(new SegmentPrivate)
{

}

//----------
Segment::Segment(const Waypt& wayptPrev, const Waypt& wayptNext, int id)
    :d_ptr(new SegmentPrivate)
{
    set(wayptPrev, wayptNext, id);
}

//----------
Segment::~Segment()
{

}

//----------
Segment::Segment(const Segment& other)
    :d_ptr(other.d_ptr)
{

}

//----------
Segment& Segment::operator=(const Segment& other)
{
    if(this != &other){
        d_ptr = other.d_ptr;
    }
    return(*this);
}

//----------
void Segment::set(const Waypt& wayptPrev, const Waypt& wayptNext, int id)
{
    setId(id);
    setWayptPrev(wayptPrev);
    setWayptNext(wayptNext);
}

//----------
void Segment::setId(int id)
{
    d_ptr->m_id = id;
}

//----------
int Segment::id() const
{
    return(d_ptr->m_id);
}

//----------
void Segment::setWayptPrev(const Waypt& wayptPrev)
{
    d_ptr->m_wayptPrev = wayptPrev;
}

//----------
const Waypt& Segment::wayptPrev() const
{
    return(d_ptr->m_wayptPrev);
}

//----------
void Segment::setWayptNext(const Waypt& wayptNext)
{
    d_ptr->m_wayptNext = wayptNext;
}

//----------
const Waypt& Segment::wayptNext() const
{
    return(d_ptr->m_wayptNext);
}

//----------
void Segment::setTVec(const VectorF& tVec)
{
    d_ptr->m_tVec = tVec;
}

//----------
const VectorF& Segment::tVec() const
{
    return(d_ptr->m_tVec);
}

//----------
void Segment::setNVec(const VectorF& nVec)
{
    d_ptr->m_nVec = nVec;
}

//----------
const VectorF& Segment::nVec() const
{
    return(d_ptr->m_nVec);
}

//----------
void Segment::setLength(double length)
{
    d_ptr->m_length = length;
}

//----------
double Segment::length() const
{
    return(d_ptr->m_length);
}

//----------
void Segment::setLengthCumulative(double lengthCumulative)
{
    d_ptr->m_lengthCumulative = lengthCumulative;
}

//----------
double Segment::lengthCumulative() const
{
    return(d_ptr->m_lengthCumulative);
}

//----------
void Segment::setbVecPrev(const VectorF& bVecPrev)
{
    d_ptr->m_bVecPrev = bVecPrev;
}

//----------
void Segment::setbVecPrev(const Segment& segmentPrev)
{
    d_ptr->calculateBisector(segmentPrev, *this, d_ptr->m_bVecPrev);
}

//----------
const VectorF& Segment::bVecPrev() const
{
    return(d_ptr->m_bVecPrev);
}

//----------
void Segment::setbVecNext(const VectorF& bVecNext)
{
    d_ptr->m_bVecNext = bVecNext;
}

//----------
void Segment::setbVecNext(const Segment& segmentNext)
{
    d_ptr->calculateBisector(*this, segmentNext, d_ptr->m_bVecNext);
}

//----------
const VectorF& Segment::bVecNext() const
{
    return(d_ptr->m_bVecNext);
}

//----------
void Segment::setSegmentAttributes()
{
    //tVec and length
    VectorF vecPrev2Next = VectorFHelper::subtract_vector(d_ptr->m_wayptNext.coord_const_ref(),
                                                          d_ptr->m_wayptPrev.coord_const_ref());
    d_ptr->m_length = VectorFHelper::norm2(vecPrev2Next);
    if(d_ptr->m_length > TOL_SMALL){
        d_ptr->m_tVec = VectorFHelper::multiply_value(vecPrev2Next, 1/d_ptr->m_length);

        //nVec
        if(DIM_COORD == 2){
            d_ptr->m_nVec[IDX_NORTHING] = -d_ptr->m_tVec[IDX_EASTING];
            d_ptr->m_nVec[IDX_EASTING] = d_ptr->m_tVec[IDX_NORTHING];
        }
        else if(DIM_COORD > 2){
            qFatal("[Segment::setSegmentAttributes()] DIM_COORD > 2 could not handled yet.");
        }
    }
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::Segment &data)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "\n===============" << "\nSegment id: " << data.id() << "\n===============\n  WayptPrev: " << data.wayptPrev() << \
                       "\n  WayptNext: " << data.wayptNext() << "\n  Length: " << data.length() << \
                       "\n  LengthCumulative: " << data.lengthCumulative();

    return debug;
}


RRTPLANNER_FRAMEWORK_END_NAMESPACE

