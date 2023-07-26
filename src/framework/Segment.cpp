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
    return;
}

//##############################
//----------
Segment::Segment()
    :mp_pimpl(new SegmentPrivate)
{

}

//----------
Segment::Segment(const Waypt& wayptPrev, const Waypt& wayptNext, int id)
    :mp_pimpl(new SegmentPrivate)
{
    set(wayptPrev, wayptNext, id);
}

//----------
Segment::~Segment()
{

}

//----------
Segment::Segment(const Segment& other)
    :mp_pimpl(other.mp_pimpl)
{

}

//----------
Segment& Segment::operator=(const Segment& other)
{
    if(this != &other){
        mp_pimpl = other.mp_pimpl;
    }
    return(*this);
}

//----------
void Segment::set(const Waypt& wayptPrev, const Waypt& wayptNext, int id)
{
    setId(id);
    setWayptPrev(wayptPrev);
    setWayptNext(wayptNext);
    return;
}

//----------
void Segment::setId(int id)
{
    mp_pimpl->m_id = id;
    return;
}

//----------
int Segment::id() const
{
    return(mp_pimpl->m_id);
}

//----------
void Segment::setWayptPrev(const Waypt& wayptPrev)
{
    mp_pimpl->m_wayptPrev = wayptPrev;
    return;
}

//----------
const Waypt& Segment::wayptPrev() const
{
    return(mp_pimpl->m_wayptPrev);
}

//----------
void Segment::setWayptNext(const Waypt& wayptNext)
{
    mp_pimpl->m_wayptNext = wayptNext;
    return;
}

//----------
const Waypt& Segment::wayptNext() const
{
    return(mp_pimpl->m_wayptNext);
}

//----------
void Segment::setTVec(const VectorF& tVec)
{
    mp_pimpl->m_tVec = tVec;
    return;
}

//----------
const VectorF& Segment::tVec() const
{
    return(mp_pimpl->m_tVec);
}

//----------
void Segment::setNVec(const VectorF& nVec)
{
    mp_pimpl->m_nVec = nVec;
    return;
}

//----------
const VectorF& Segment::nVec() const
{
    return(mp_pimpl->m_nVec);
}

//----------
void Segment::setLength(double length)
{
    mp_pimpl->m_length = length;
    return;
}

//----------
double Segment::length() const
{
    return(mp_pimpl->m_length);
}

//----------
void Segment::setLengthCumulative(double lengthCumulative)
{
    mp_pimpl->m_lengthCumulative = lengthCumulative;
    return;
}

//----------
double Segment::lengthCumulative() const
{
    return(mp_pimpl->m_lengthCumulative);
}

//----------
void Segment::setbVecPrev(const VectorF& bVecPrev)
{
    mp_pimpl->m_bVecPrev = bVecPrev;
    return;
}

//----------
void Segment::setbVecPrev(const Segment& segmentPrev)
{
    mp_pimpl->calculateBisector(segmentPrev, *this, mp_pimpl->m_bVecPrev);
    return;
}

//----------
const VectorF& Segment::bVecPrev() const
{
    return(mp_pimpl->m_bVecPrev);
}

//----------
void Segment::setbVecNext(const VectorF& bVecNext)
{
    mp_pimpl->m_bVecNext = bVecNext;
    return;
}

//----------
void Segment::setbVecNext(const Segment& segmentNext)
{
    mp_pimpl->calculateBisector(*this, segmentNext, mp_pimpl->m_bVecNext);
    return;
}

//----------
const VectorF& Segment::bVecNext() const
{
    return(mp_pimpl->m_bVecNext);
}

//----------
void Segment::setSegmentAttributes()
{
    //tVec and length
    VectorF vecPrev2Next = VectorFHelper::subtract_vector(mp_pimpl->m_wayptNext.coord_const_ref(),
                                                          mp_pimpl->m_wayptPrev.coord_const_ref());
    mp_pimpl->m_length = VectorFHelper::norm2(vecPrev2Next);
    if(mp_pimpl->m_length > TOL_SMALL){
        mp_pimpl->m_tVec = VectorFHelper::multiply_value(vecPrev2Next, 1/mp_pimpl->m_length);

        //nVec
        if(DIM_COORD == 2){
            mp_pimpl->m_nVec[IDX_NORTHING] = -mp_pimpl->m_tVec[IDX_EASTING];
            mp_pimpl->m_nVec[IDX_EASTING] = mp_pimpl->m_tVec[IDX_NORTHING];
        }
        else if(DIM_COORD > 2){
            qFatal("[Segment::setSegmentAttributes()] DIM_COORD > 2 could not handled yet.");
        }
    }
    return;
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

