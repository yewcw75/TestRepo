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
    ~SegmentPrivate()
    {}
    SegmentPrivate(const SegmentPrivate& other)
        : QSharedData(other),
          m_wayptPrev(other.m_wayptPrev),
          m_wayptNext(other.m_wayptNext),
          m_tVec(other.m_tVec),
          m_nVec(other.m_nVec),
          m_bVecPrev(other.m_bVecPrev),
          m_bVecNext(other.m_bVecNext),
          m_length(other.m_length),
          m_lengthCumulative(other.m_lengthCumulative),
          m_id(other.m_id),
          m_isZeroLengthSegment(other.m_isZeroLengthSegment)
    {}
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
    bool m_isZeroLengthSegment{};
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
Segment::Segment(const Waypt& wayptPrev, const Waypt& wayptNext, int id, bool isZeroLengthSegment)
    :mp_pimpl(new SegmentPrivate)
{
    set(wayptPrev, wayptNext, id, isZeroLengthSegment);
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
void Segment::set(const Waypt& wayptPrev, const Waypt& wayptNext, int id, bool isZeroLengthSegment)
{
    setId(id);
    mp_pimpl->m_isZeroLengthSegment = isZeroLengthSegment;
    if(isZeroLengthSegment){
        bool ok = (wayptPrev.easting() - wayptNext.easting() < TOL_SMALL && \
                 wayptPrev.northing() - wayptNext.northing() < TOL_SMALL && \
                 wayptPrev.lon0_deg() - wayptNext.lon0_deg() < TOL_SMALL);
        if(!ok){
            qWarning() << \
            "[Segment::set(const Waypt&, const Waypt&, int, bool)] specified to be a zero-segment but wayptPrev and wayptNext are not at the same pt.";
        }
    }
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
void Segment::setIsZeroLengthSegment(bool isZeroLengthSegment)
{
    mp_pimpl->m_isZeroLengthSegment = isZeroLengthSegment;
    return;
}

//----------
bool Segment::isZeroLengthSegment() const
{
    return(mp_pimpl->m_isZeroLengthSegment);
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
    VectorF bVecPrev2Use = bVecPrev;
    double norm2 = VectorFHelper::norm2(bVecPrev2Use);
    if(!UtilHelper::compare(norm2, (double)1.0)){
        qWarning() << "[Segment::setbVecPrev(const VectorF& bVecPrev)] bVecPres expected to be a unit vector but it is not! Normalization performed.";
        bVecPrev2Use = VectorFHelper::multiply_value(bVecPrev2Use, 1/norm2);
    }

    mp_pimpl->m_bVecPrev = bVecPrev2Use;
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
    VectorF bVecNext2Use = bVecNext;
    double norm2 = VectorFHelper::norm2(bVecNext2Use);
    if(!UtilHelper::compare(norm2, (double)1.0)){
        qWarning() << "[setbVecNext(const VectorF& bVecNext)] bVecNext expected to be a unit vector but it is not! Normalization performed.";
        bVecNext2Use = VectorFHelper::multiply_value(bVecNext2Use, 1/norm2);
    }
    mp_pimpl->m_bVecNext = bVecNext2Use;
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
    if(mp_pimpl->m_isZeroLengthSegment){
        mp_pimpl->m_length = 0.0;
    }
    else{
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
    }
    return;
}


RRTPLANNER_FRAMEWORK_END_NAMESPACE

