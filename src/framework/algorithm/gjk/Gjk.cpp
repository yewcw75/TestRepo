#include <RrtPlannerLib/framework/algorithm/gjk/Gjk.h>
#include <RrtPlannerLib/framework/algorithm/gjk/Simplex.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QDebug>

#define MAX_ITER 1000
#define EPS_REL 1e-6
#define EPS_TOT 1e-6

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkPrivate
{
public:
    GjkPrivate()
    {}
    ~GjkPrivate()
    {}

    //note: searchDir need not be normalized to unit vector.
    //return flag:  0 => spp on origin,  -1 => spp misses origin, 1 => spp is beyond origin
    int support(const IShape& shape1, const IShape& shape2, const VectorF& searchDir, VectorF& support);

public:
    int m_maxIter{MAX_ITER}; //Max number of iterations
    double eps_rel{EPS_REL}; //Termination condition
    double eps_tot{EPS_TOT}; //Termination condition
};

//v is the search dir.
//spp is the output support point of the minkowski difference in the search dir.
//note: searchDir, v, need not be normalized to unit vector.
int GjkPrivate::support(const IShape& shape1, const IShape& shape2, const VectorF& v, VectorF& spp)
{
    VectorF spp1 = shape1.support(VectorFHelper::multiply_value(v, -1.0));
    VectorF spp2 = shape2.support(v);
    spp = VectorFHelper::subtract_vector(spp2, spp1);

    //check if support is at origin
    int result_flag = 1; //spp beyond origin
    if( VectorFHelper::dot_product(spp, spp) < eps_rel) { //equivalent to |spp| < sqrt(eps_rel)
        result_flag = 0; //spp at origin
    }
    else{
        double spp_dot_v = VectorFHelper::dot_product(spp, v);
        double v_dot_v = VectorFHelper::dot_product(v, v);
        if(spp_dot_v < 0 && spp_dot_v*spp_dot_v > eps_rel*v_dot_v) {// equivalent to (spp.v)/|v| < -sqrt(eps_rel)
            result_flag = -1; //spp misses origin => both polygons do not intersect
        }
    }
    return(result_flag);
}

//####################

QScopedPointer<GjkPrivate> Gjk::mp_pimpl(new GjkPrivate);

//----------
Gjk::Gjk()
{

}

//----------
Gjk::~Gjk()
{

}

//----------
bool Gjk::chkIntersect(const IShape& shape1, const IShape& shape2)
{
    bool isIntersect = false;

    //initialize simplex
    Simplex simplex;
    VectorF searchDir = VectorFHelper::subtract_vector(shape2.centroid(), shape1.centroid()); //arbitrary search dir
    VectorF spp0;
    int result_flag = mp_pimpl->support(shape1, shape2, searchDir, spp0);

    isIntersect = result_flag == 0; //spp on origin
    if(result_flag == 1){ //1 means that spp is beyond origin
        simplex.update(spp0, searchDir);  //will call handle0D(v) in Simplex class. return searchDir as Origin - vertex[0]. bool return is ignored here.

        //iteration
        int k = 0;
        while(k++ < mp_pimpl->m_maxIter){
            VectorF spp;
            int result_flag = mp_pimpl->support(shape1, shape2, searchDir, spp);
            isIntersect = result_flag == 0;
            if(isIntersect || result_flag == -1) { //-1 =>miss origin
                break;
            }

            isIntersect = simplex.update(spp, searchDir); //note searchDir is updated with the next search direction to use
            if(isIntersect){
                break;
            }
        }

        //throws warning if max iteration was reached
        if(k > mp_pimpl->m_maxIter - 1){
            qWarning() << "[Gjk::chkIntersect] Maximum iteration reached while searching for origin in simplex. Results may not be accurate!";
        }
    } //if(result_flag == 1)
    return(isIntersect);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
