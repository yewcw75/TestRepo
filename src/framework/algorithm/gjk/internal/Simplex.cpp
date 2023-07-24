#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QVector>

#define EPS_SQUARE_DEFAULT 1e-6

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SimplexPrivate
{
public:
    SimplexPrivate()
    {}
    SimplexPrivate(const SimplexPrivate& other) = delete; //non-copyable
    ~SimplexPrivate()
    {}

public:
    double m_eps_square{EPS_SQUARE_DEFAULT};
};

//###################
//----------
Simplex::Simplex()
    :mp_pimpl(new SimplexPrivate)
{

}

//----------
Simplex::Simplex(double eps_square)
    :mp_pimpl(new SimplexPrivate)
{
    set_eps_square(eps_square);
}

//----------
Simplex::~Simplex()
{

}

//----------
void Simplex::set_eps_square(double eps_square)
{
    mp_pimpl->m_eps_square = eps_square;
    return;
}

//----------
double Simplex::eps_square() const
{
    return(mp_pimpl->m_eps_square);
}


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
