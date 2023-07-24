#include <RrtPlannerLib/framework/algorithm/gjk/internal/Support.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QVector>

#define EPS_SQUARE_DEFAULT 1e-6

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SupportPrivate
{
public:
    SupportPrivate()
    {}
    SupportPrivate(const SupportPrivate& other) = delete; //non-copyable
    ~SupportPrivate()
    {}

public:
    double m_eps_square{EPS_SQUARE_DEFAULT};
};

//###################
//----------
Support::Support()
    :mp_pimpl(new SupportPrivate)
{

}

//----------
Support::Support(double eps_square)
    :mp_pimpl(new SupportPrivate)
{
    set_eps_square(eps_square);
}

//----------
Support::~Support()
{

}

//----------
void Support::set_eps_square(double eps_square)
{
    mp_pimpl->m_eps_square = eps_square;
    return;
}

//----------
double Support::eps_square() const
{
    return(mp_pimpl->m_eps_square);
}


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
