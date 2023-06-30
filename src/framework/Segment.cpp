#include <RrtPlannerLib/framework/Segment.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
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
          m_fieldFlags(other.m_fieldFlags),
          m_length(other.m_length),
          m_id(other.m_id),
          m_isZeroLengthSegment(other.m_isZeroLengthSegment)
    {}

    void checkAndSetSegmentAttributes();
    void calculateBisector(const Segment& seg1, const Segment& seg2, VectorF& bVec);

public:
    Waypt m_wayptPrev;
    Waypt m_wayptNext;
    VectorF m_tVec;
    VectorF m_nVec;
    VectorF m_bVecPrev;
    VectorF m_bVecNext;
    Segment::FieldFlags m_fieldFlags{};
    float m_length{};
    int m_id{-1};
    bool m_isZeroLengthSegment{};
};

//----------
void SegmentPrivate::checkAndSetSegmentAttributes()
{
    if(m_isZeroLengthSegment){
        m_length = 0.0;
        m_fieldFlags.setFlag(Segment::Field::LENGTH);
    }
    else if(m_fieldFlags.testFlag(Segment::Field::WAYPT_PREV) && \
        m_fieldFlags.testFlag(Segment::Field::WAYPT_NEXT))
    {
        //tVec and length
        VectorF vecPrev2Next = VectorFHelper::subtract_vector(m_wayptNext.coord_const_ref(), m_wayptPrev.coord_const_ref());
        m_length = VectorFHelper::norm2(vecPrev2Next);
        m_fieldFlags.setFlag(Segment::Field::LENGTH);
        if(m_length > TOL_SMALL){
            m_tVec = VectorFHelper::multiply_value(vecPrev2Next, 1/m_length);
            m_fieldFlags.setFlag(Segment::Field::TVEC);

            //nVec
            if(DIM_COORD == 2){
                m_nVec[IDX_NORTHING] = -m_tVec[IDX_EASTING];
                m_nVec[IDX_EASTING] = m_tVec[IDX_NORTHING];
                m_fieldFlags.setFlag(Segment::Field::NVEC);
            }
            else if(DIM_COORD > 2){
                Q_ASSERT(0); //to be handled
            }
        }
    }
}

//----------
void SegmentPrivate::calculateBisector(const Segment& seg1, const Segment& seg2, VectorF& bVec)
{
    Q_ASSERT(seg1.getFieldFlags().testFlag(Segment::Field::NVEC));
    Q_ASSERT(seg2.getFieldFlags().testFlag(Segment::Field::NVEC));

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
        Q_ASSERT(wayptPrev.easting() - wayptNext.easting() < TOL_SMALL && \
                 wayptPrev.northing() - wayptNext.northing() < TOL_SMALL && \
                 wayptPrev.lon0_deg() - wayptNext.lon0_deg() < TOL_SMALL);
    }
    setWayptPrev(wayptPrev);
    setWayptNext(wayptNext);
    return;
}

//----------
void Segment::setId(int id)
{
    mp_pimpl->m_id = id;
    mp_pimpl->m_fieldFlags.setFlag(Field::ID);
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
    mp_pimpl->m_fieldFlags.setFlag(Field::WAYPT_PREV);
    mp_pimpl->checkAndSetSegmentAttributes();
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
    mp_pimpl->m_fieldFlags.setFlag(Field::WAYPT_NEXT);
    mp_pimpl->checkAndSetSegmentAttributes();
    return;
}

//----------
const Waypt& Segment::wayptNext() const
{
    return(mp_pimpl->m_wayptNext);
}

//----------
const VectorF& Segment::tVec() const
{
    return(mp_pimpl->m_tVec);
}

//----------
const VectorF& Segment::nVec() const
{
    return(mp_pimpl->m_nVec);
}

//----------
float Segment::length() const
{
    return(mp_pimpl->m_length);
}

//----------
void Segment::setbVecPrev(const VectorF& bVecPrev)
{
    float norm2 = VectorFHelper::norm2(bVecPrev);
    Q_ASSERT(qFuzzyCompare(norm2, (float)1.0));

    mp_pimpl->m_bVecPrev = bVecPrev;
    mp_pimpl->m_fieldFlags.setFlag(Field::BVEC_PREV);
    return;
}

//----------
void Segment::setbVecPrev(const Segment& segmentPrev)
{
    mp_pimpl->calculateBisector(segmentPrev, *this, mp_pimpl->m_bVecPrev);
    mp_pimpl->m_fieldFlags.setFlag(Field::BVEC_PREV);
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
    float norm2 = VectorFHelper::norm2(bVecNext);
    Q_ASSERT(qFuzzyCompare(norm2, (float)1.0));

    mp_pimpl->m_bVecNext = bVecNext;
    mp_pimpl->m_fieldFlags.setFlag(Field::BVEC_NEXT);
    return;
}

//----------
void Segment::setbVecNext(const Segment& segmentNext)
{
    mp_pimpl->calculateBisector(*this, segmentNext, mp_pimpl->m_bVecNext);
    mp_pimpl->m_fieldFlags.setFlag(Field::BVEC_NEXT);
    return;
}

//----------
const VectorF& Segment::bVecNext() const
{
    return(mp_pimpl->m_bVecNext);
}

//----------
const Segment::FieldFlags& Segment::getFieldFlags() const
{
    return(mp_pimpl->m_fieldFlags);
}
//----------


RRTPLANNER_FRAMEWORK_END_NAMESPACE

