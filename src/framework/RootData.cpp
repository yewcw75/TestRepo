#include <RrtPlannerLib/framework/RootData.h>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class RootDataPrivate : public QSharedData
{
public:
    RootDataPrivate() = default;
    RootDataPrivate(const RootDataPrivate& other) = default;
    ~RootDataPrivate() = default;

    void reset();

public:
    VectorF m_posNE{}; //usv position.
    QVector<double> m_ell_list; //USV arclength baseline


    //params of offset plan at usv position.
    double m_dx{}; //[m] cross-track dist. wrt nominal plan.
    double m_ell{}; //[m] length from start to current usv position.
    double m_L{}; //[m] length of segment that usv position is on.
    double m_f_ell{}; //fraction dl/L, where dl is the distance from start of current segment to usv position.

    //params for the polygon sector in which the root is inside.
    int m_planIdx{-1}; //plan idx associated with the polygon sector that root is inside.
    int m_segIdx{-1}; //segment idx associated with the polygon sector that root is inside.
    bool m_isInPoly{}; //usv position is within the span of all the plans in EllMap.
};

//----------
void RootDataPrivate::reset()
{
    m_posNE = VectorF{};
    m_ell_list.clear();
    m_dx = 0.0;
    m_ell = 0.0;
    m_L = 0.0;
    m_f_ell = 0.0;
    m_planIdx = -1;
    m_segIdx = -1;
    m_isInPoly = false;
}

//######################
//----------
RootData::RootData()
    : d_ptr(new RootDataPrivate)
{

}

//----------
RootData::RootData(const VectorF &posNE)
    : RootData()
{
    setPosNE(posNE);
}

//----------
RootData::RootData(const RootData &other)
    : d_ptr(other.d_ptr)
{

}

//----------
RootData &RootData::operator=(const RootData &other)
{
    if (this != &other){
        d_ptr = other.d_ptr;
    }
    return *this;
}

//----------
RootData::~RootData()
{

}

//----------
QVector<double>& RootData::ell_list()
{
    return d_ptr->m_ell_list;
}

//----------
const QVector<double>& RootData::ell_list_const_ref() const
{
    return d_ptr->m_ell_list;
}

//----------
double RootData::dx() const
{
    return d_ptr->m_dx;
}

//----------
void RootData::setDx(double dx)
{
    d_ptr->m_dx = dx;
}

//----------
double RootData::ell() const
{
    return d_ptr->m_ell;
}

//----------
void RootData::setEll(double ell)
{
    d_ptr->m_ell = ell;
}

//----------
double RootData::L() const
{
    return d_ptr->m_L;
}

//----------
void RootData::setL(double l)
{
    d_ptr->m_L = l;
}

//----------
double RootData::f_ell() const
{
    return d_ptr->m_f_ell;
}

//----------
void RootData::setF_ell(double f_ell)
{
    d_ptr->m_f_ell = f_ell;
}

//----------
int RootData::planIdx() const
{
    return d_ptr->m_planIdx;
}

//----------
void RootData::setPlanIdx(int planIdx)
{
    d_ptr->m_planIdx = planIdx;
}

//----------
int RootData::segIdx() const
{
    return d_ptr->m_segIdx;
}

//----------
void RootData::setSegIdx(int segIdx)
{
    d_ptr->m_segIdx = segIdx;
}

//----------
bool RootData::isInPoly() const
{
    return d_ptr->m_isInPoly;
}

//----------
void RootData::setIsInPoly(bool isInPoly)
{
    d_ptr->m_isInPoly = isInPoly;
}

//----------
void RootData::reset()
{
    d_ptr->reset();
}

//----------
const VectorF &RootData::posNE() const
{
    return d_ptr->m_posNE;
}

//----------
void RootData::setPosNE(const VectorF& posNE)
{
    d_ptr->m_posNE = posNE;
}

//----------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::RootData &data)
{
    QString ellListStr;
    const QVector<double>& ellList = data.ell_list_const_ref();
    for(int i = 0; i < ellList.size(); ++i){
        if(i > 0){
            ellListStr += ", ";
        }
        ellListStr += QString::number(ellList.at(i), 'f', 2);
    }

    QDebugStateSaver saver(debug);
    debug.nospace() << "\n  PosNE = " << data.posNE() << \
                       "\n  dx = " << data.dx() << "m" << \
                       "\n  ell = " << data.ell() << "m" << \
                       "\n  L = " << data.L() << "m" << \
                       "\n  f_ell = " << data.f_ell() << \
                       "\n  planIdx = " << data.planIdx() << \
                       "\n  segIdx = " << data.segIdx() << \
                       "\n  isInPoly = " << data.isInPoly() << \
                       "\n  ellList = " << ellListStr;

    return debug;
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
