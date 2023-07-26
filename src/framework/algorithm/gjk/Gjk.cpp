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
    GjkPrivate() = default;
    GjkPrivate(const GjkPrivate& other) = delete;
    ~GjkPrivate() = default;

public:
    double m_eps_square{EPS_SQUARE};
    int m_maxIter{MAX_ITER}; //Max number of iterations
};

//####################

//----------
Gjk::Gjk()
    :d_ptr(new GjkPrivate)
{

}

//----------
Gjk::~Gjk()
{

}

//----------
void Gjk::set_eps_square(double eps_square)
{
    d_ptr->m_eps_square = eps_square;
}

//----------
double Gjk::eps_square() const
{
    return(d_ptr->m_eps_square);
}

//----------
void Gjk::set_max_iteration(int max_iteration)
{
    d_ptr->m_maxIter = max_iteration;
}

//----------
int Gjk::max_iteration() const
{
    return(d_ptr->m_maxIter);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
