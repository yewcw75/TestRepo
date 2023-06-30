#include "RrtPlannerLib/framework/FrameworkDefines.h"
#include <RrtPlannerLib/framework/UblasHelper.h>
#include <QtGlobal>
#include <boost/numeric/ublas/lu.hpp>

namespace bnu = boost::numeric::ublas;

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//----------
UblasHelper::UblasHelper()
{

}

//----------
UblasHelper::~UblasHelper()
{

}

//----------
bnu::matrix<float> UblasHelper::concatenate_col_vectors(const bnu::vector<float>& v1,
                                                      const bnu::vector<float>& v2)
{
    Q_ASSERT(v1.size() == v2.size());
    int nRow = v1.size();

    bnu::matrix<float> ret(nRow, 2);
    for(int i = 0; i < nRow; ++i){
        ret(i, 0) = v1(i);
        ret(i, 1) = v2(i);
    }
    return(ret);
}

//----------
bool UblasHelper::solve(const bnu::matrix<float>& M, //invertible square matrix, [NxN]
                                const bnu::vector<float>& y, //[Nx1]
                                float tol_small, //for checking if ok to 1/det(M)
                                bnu::vector<float>& x //[Nx1]
                                )
{
    bool res = true;

    //verify input
    int dim = y.size();
    Q_ASSERT(static_cast<int>(M.size1()) == dim && \
             static_cast<int>(M.size2()) == dim);

    //for dim = 2 case
    if(dim == 2){
        float a = M(0, 0), b = M(0, 1), c = M(1, 0), d = M(1, 1);
        float det = a*d - c*b;
        if(abs(det) > tol_small){
            bnu::matrix<float> M_aux(2, 2);
            M_aux(0, 0) = d; M_aux(0, 1) = -b; M_aux(1, 0) = -c; M_aux(1, 1) = a;
            bnu::matrix<float> invM = (1/det) * M_aux;
            x = bnu::prod(invM, y);
        }
        else{
            res = false;
        }
    }
    else{
        // Perform LU factorization
        boost::numeric::ublas::matrix<float> M2 = M;
        bnu::permutation_matrix<std::size_t> permutation(M2.size1());

        int result = bnu::lu_factorize(M2, permutation); //res = 0 if M2 is not singular
        if (result == 0){
            // Compute the inverse of the matrix
            bnu::matrix<float> M2_inv(M2.size1(), M2.size2());
            bnu::identity_matrix<float> I(M2.size1());
            bnu::lu_substitute(M2, permutation, M2_inv);
        }
        else{
            res = false;
        }
    }
    return(res);
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
