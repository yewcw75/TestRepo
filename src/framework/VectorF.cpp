#include <RrtPlannerLib/framework/VectorF.h>
#include <QSharedData>
#include <QScopedPointer>

namespace bnu = boost::numeric::ublas;

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

class VectorFPrivate: public QSharedData
{
public:
    explicit VectorFPrivate(int size = 0)
        :QSharedData(),
          m_data(new bnu::vector<double>(size))
    {}
    explicit VectorFPrivate(const bnu::vector<double>& data)
        :QSharedData(),
          m_data(new bnu::vector<double>(data))
    {}
    VectorFPrivate(const VectorFPrivate& other)
        : QSharedData(other),
          m_data(new bnu::vector<double>(*other.m_data))
    {}
    ~VectorFPrivate() = default;

public:
    QScopedPointer<bnu::vector<double>> m_data;
};

//#########################
//---------
VectorF::VectorF()
    :d_ptr(new VectorFPrivate())
{

}

//---------
VectorF::VectorF(const std::initializer_list<double>& list)
    :d_ptr(new VectorFPrivate)
{
    size_t size = list.size();
    d_ptr->m_data->resize(size);

    size_t index = 0;
    for (const auto& value : list) {
         (*d_ptr->m_data)[index++] = value;
    }
}

//---------
VectorF::VectorF(const boost::numeric::ublas::vector<double>& data)
    :d_ptr(new VectorFPrivate(data))
{

}

//---------
VectorF::~VectorF()
{

}

//---------
VectorF::VectorF(const VectorF& other)
    :d_ptr(other.d_ptr)
{

}

//---------
VectorF& VectorF::operator=(const VectorF& other)
{
    if(this != &other){
        d_ptr = other.d_ptr;
    }
    return(*this);
}

//---------
void VectorF::resize(int size, bool to_preserve_data)
{
    d_ptr->m_data->resize(size, to_preserve_data);
}

//---------
int VectorF::size() const
{
    return(d_ptr->m_data->size());
}

//---------
double VectorF::at(int idx) const
{
    return((*d_ptr->m_data)[idx]);
}

//---------
double VectorF::operator[](int idx) const
{
    return((*d_ptr->m_data)[idx]);
}

//---------
double& VectorF::operator[](int idx)
{
    return((*d_ptr->m_data)[idx]);
}

//---------
void VectorF::set(const boost::numeric::ublas::vector<double>& data)
{
    *(d_ptr->m_data) = data;
}

//---------
const boost::numeric::ublas::vector<double>& VectorF::data_const_ref() const
{
    return(*(d_ptr->m_data));
}

//---------
boost::numeric::ublas::vector<double>& VectorF::data()
{
    return(*(d_ptr->m_data));
}

//---------
QDebug operator<<(QDebug debug, const RRTPLANNER_NAMESPACE::framework::VectorF &data)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "(";
    for(int i = 0; i < data.size(); ++i){
        if(i != 0) debug.nospace() << ",";
        debug.nospace() << data[i];
    }
    debug.nospace() << ")";
    return debug;
}


RRTPLANNER_FRAMEWORK_END_NAMESPACE


