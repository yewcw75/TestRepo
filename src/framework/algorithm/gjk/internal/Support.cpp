#include <RrtPlannerLib/framework/algorithm/gjk/internal/Support.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QVector>

#define EPS_SQUARE_DEFAULT 1e-6

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SupportPrivate
{
public:
    SupportPrivate() = default;
    SupportPrivate(const SupportPrivate& other) = delete; //non-copyable
    ~SupportPrivate() = default;

public:
    double m_eps_square{EPS_SQUARE_DEFAULT};
};

//###################
//----------
Support::Support()
    :d_ptr(new SupportPrivate)
{

}

//----------
Support::Support(double eps_square)
    :d_ptr(new SupportPrivate)
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
    d_ptr->m_eps_square = eps_square;
}

//----------
double Support::eps_square() const
{
    return(d_ptr->m_eps_square);
}


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
