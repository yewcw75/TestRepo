#ifndef RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_H
#define RRTPLANNERLIB_FRAMEWORK_ALGORITHM_GJK_SIMPLEX_H

#include <RrtPlannerLib/RrtPlannerLibGlobal.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/algorithm/gjk/IShape.h>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class SimplexPrivate;
class RRTPLANNER_LIB_EXPORT_FOR_BUILDTEST Simplex
{
public:
    Simplex();
    virtual ~Simplex();
    Simplex(const Simplex& other) = delete; //non-copyable class

    //update simplex with a new support point
    //return bool to indicate if origin is in simplex
    //v is a vector along the next search dir (un-normalized)
    //input vertex is assumed to NOT be at origin.
    bool update(const VectorF& vertex, VectorF& v);

private:
    QScopedPointer<SimplexPrivate> mp_pimpl;
};

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE

#endif
