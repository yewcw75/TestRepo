#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/Segment.h>
#include <RrtPlannerLib/framework/GeometryHelper.h>
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
    {}
    ~SegmentPrivate()
    {}
    SegmentPrivate(const SegmentPrivate& other)
        : QSharedData(other),
          m_nodePrev(other.m_nodePrev),
          m_nodeNext(other.m_nodeNext),
          m_tVec(other.m_tVec),
          m_nVec(other.m_nVec),
          m_bVecPrev(other.m_bVecPrev),
          m_bVecNext(other.m_bVecNext),
          m_fieldFlags(other.m_fieldFlags),
          m_length(other.m_length),
          m_id(other.m_id)
    {}

    void checkAndSetSegmentAttributes();
    void calculateBisector(const Segment& seg1, const Segment& seg2, Vector_NE& bVec);

public:
    Waypt m_nodePrev{};
    Waypt m_nodeNext{};
    Vector_NE m_tVec{};
    Vector_NE m_nVec{};
    Vector_NE m_bVecPrev{};
    Vector_NE m_bVecNext{};
    Segment::FieldFlags m_fieldFlags{};
    float m_length{};
    int m_id{-1};
};

//----------
void SegmentPrivate::checkAndSetSegmentAttributes()
{
    if(m_fieldFlags.testFlag(Segment::Field::NODE_PREV) && \
            m_fieldFlags.testFlag(Segment::Field::NODE_NEXT))
    {
        //tVec and length
        Vector_NE vecPrev2Next = m_nodePrev.vectorTo(m_nodeNext);
        m_length = GeometryHelper::lengthVector(vecPrev2Next);
        m_fieldFlags.setFlag(Segment::Field::LENGTH);
        if(m_length > TOL_SMALL){
            m_tVec = GeometryHelper::scalar_multiplyVector(1/m_length, vecPrev2Next);
            m_fieldFlags.setFlag(Segment::Field::TVEC);

            //nVec
            if(DIM_COORD == 2){
                m_nVec.set<IDX_NORTHING>(-m_tVec.get<IDX_EASTING>());
                m_nVec.set<IDX_EASTING>(m_tVec.get<IDX_NORTHING>());
                m_fieldFlags.setFlag(Segment::Field::NVEC);
            }
            else if(DIM_COORD > 2){
                Q_ASSERT(0); //to be handled
            }
        }
    }
}

//----------
void SegmentPrivate::calculateBisector(const Segment& seg1, const Segment& seg2, Vector_NE& bVec)
{
    Q_ASSERT(seg1.getFieldFlags().testFlag(Segment::Field::NVEC));
    Q_ASSERT(seg2.getFieldFlags().testFlag(Segment::Field::NVEC));

    Vector_NE v = GeometryHelper::addVector(seg1.nVec(), seg2.nVec());
    double length_v = GeometryHelper::lengthVector(v);
    if(length_v > TOL_SMALL){
        bVec = GeometryHelper::scalar_multiplyVector(1/length_v, v);
        bg::multiply_value(v, 1/length_v);
    }
    else{
        //left-hand perp vector of b (tvec)
        bVec.set<IDX_NORTHING>(seg2.nVec().get<IDX_EASTING>());
        bVec.set<IDX_EASTING>(-seg2.nVec().get<IDX_NORTHING>());
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
Segment::Segment(const Waypt& nodePrev, const Waypt& nodeNext, int id)
    :mp_pimpl(new SegmentPrivate)
{
    set(nodePrev, nodeNext, id);
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
void Segment::set(const Waypt& nodePrev, const Waypt& nodeNext, int id)
{
    setId(id);
    setNodePrev(nodePrev);
    setNodeNext(nodeNext);
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
void Segment::setNodePrev(const Waypt& nodePrev)
{
    mp_pimpl->m_nodePrev = nodePrev;
    mp_pimpl->m_fieldFlags.setFlag(Field::NODE_PREV);
    mp_pimpl->checkAndSetSegmentAttributes();
    return;
}

//----------
Waypt Segment::nodePrev() const
{
    return(mp_pimpl->m_nodePrev);
}

//----------
void Segment::setNodeNext(const Waypt& nodeNext)
{
    mp_pimpl->m_nodeNext = nodeNext;
    mp_pimpl->m_fieldFlags.setFlag(Field::NODE_NEXT);
    mp_pimpl->checkAndSetSegmentAttributes();
    return;
}

//----------
Waypt Segment::nodeNext() const
{
    return(mp_pimpl->m_nodeNext);
}

//----------
Vector_NE Segment::tVec() const
{
    return(mp_pimpl->m_tVec);
}

//----------
Vector_NE Segment::nVec() const
{
    return(mp_pimpl->m_nVec);
}

//----------
float Segment::length() const
{
    return(mp_pimpl->m_length);
}

//----------
void Segment::setbVecPrev(const Vector_NE& bVecPrev)
{
    float normSquare = bg::dot_product(bVecPrev, bVecPrev);
    Q_ASSERT(qFuzzyCompare(normSquare, (float)1.0));

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
Vector_NE Segment::bVecPrev() const
{
    return(mp_pimpl->m_bVecPrev);
}

//----------
void Segment::setbVecNext(const Vector_NE& bVecNext)
{
    float normSquare = bg::dot_product(bVecNext, bVecNext);
    Q_ASSERT(qFuzzyCompare(normSquare, (float)1.0));

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
Vector_NE Segment::bVecNext() const
{
    return(mp_pimpl->m_bVecNext);
}

//----------
Segment::FieldFlags Segment::getFieldFlags() const
{
    return(mp_pimpl->m_fieldFlags);
}
//----------


RRTPLANNER_FRAMEWORK_END_NAMESPACE

