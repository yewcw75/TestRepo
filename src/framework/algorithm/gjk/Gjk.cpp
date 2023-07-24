#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <RrtPlannerLib/framework/algorithm/gjk/GjkDefines.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Support.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QDebug>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkPrivate
{
public:
    GjkPrivate()
        :m_eps_square(EPS_SQUARE),
          m_maxIter(MAX_ITER)
    {

    }
    ~GjkPrivate()
    {}

public:
    QScopedPointer<Simplex> mp_simplex{};
    QScopedPointer<Support> mp_support{};
    double m_eps_square{};
    int m_maxIter{}; //Max number of iterations
};

//####################

//----------
Gjk::Gjk()
    :mp_pimpl(new GjkPrivate)
{

}

//----------
Gjk::~Gjk()
{

}

//----------
void Gjk::set_eps_square(double eps_square)
{
    mp_pimpl->m_eps_square = eps_square;
    return;
}

//----------
double Gjk::eps_square() const
{
    return(mp_pimpl->m_eps_square);
}

//----------
void Gjk::set_max_iteration(int max_iteration)
{
    mp_pimpl->m_maxIter = max_iteration;
    return;
}

//----------
int Gjk::max_iteration() const
{
    return(mp_pimpl->m_maxIter);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
