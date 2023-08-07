#include <RrtPlannerLib/framework/UtilHelper.h>

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//----------
UtilHelper::UtilHelper()
{

}

//----------
UtilHelper::~UtilHelper()
{

}

//----------
bool UtilHelper::compare(double d1, double d2, double tol_small)
{
    return(abs(d1 - d2) <= tol_small);
}

//----------
int UtilHelper::mod(int x, int N)
{
    return((x % N + N) % N);
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
