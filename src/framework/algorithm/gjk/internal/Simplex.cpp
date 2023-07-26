#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QVector>

#define EPS_SQUARE_DEFAULT 1e-6

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SimplexPrivate
{
public:
    SimplexPrivate() = default;
    SimplexPrivate(const SimplexPrivate& other) = delete; //non-copyable
    ~SimplexPrivate() = default;

public:
    double m_eps_square{EPS_SQUARE_DEFAULT};
};

//###################
//----------
Simplex::Simplex()
    :d_ptr(new SimplexPrivate)
{

}

//----------
Simplex::Simplex(double eps_square)
    :d_ptr(new SimplexPrivate)
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
    d_ptr->m_eps_square = eps_square;
}

//----------
double Simplex::eps_square() const
{
    return(d_ptr->m_eps_square);
}


RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
