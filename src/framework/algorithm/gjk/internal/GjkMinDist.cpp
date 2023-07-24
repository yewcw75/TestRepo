#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkMinDist.h>
#include <RrtPlannerLib/framework/algorithm/gjk/GjkDefines.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactory.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/GjkComponentFactoryCreator.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Simplex.h>
#include <RrtPlannerLib/framework/algorithm/gjk/internal/Support.h>
#include <RrtPlannerLib/framework/VectorF.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <QDebug>
#include <QScopedPointer>

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_BEGIN_NAMESPACE

class GjkMinDistPrivate
{
public:
    GjkMinDistPrivate()
    {}
    ~GjkMinDistPrivate()
    {}

public:
    QScopedPointer<Simplex> mp_simplex;
    QScopedPointer<Support> mp_support;

};

//####################

//----------
GjkMinDist::GjkMinDist()
    :Gjk(),
      mp_pimpl(new GjkMinDistPrivate)
{
    QScopedPointer<GjkComponentFactory> gjkFactory(GjkComponentFactoryCreator::getGjkComponentFactory(GjkComponentFactoryCreator::GjkType::MinDist));
    mp_pimpl->mp_simplex.reset(gjkFactory->getSimplex(eps_square()));
    mp_pimpl->mp_support.reset(gjkFactory->getSupport(eps_square()));
}

//----------
GjkMinDist::~GjkMinDist()
{

}

//----------
bool GjkMinDist::chkIntersect(const IShape& shape1, const IShape& shape2,
                       double& distance, //min distance between the shapes if they do not intersect
                       bool& isValidDistance //to indicate whether distance if valid. Note, if input shape intersect, this param shd return false as we do not calculate the distance for intersecting shapes.
                       )
{

    bool isIntersect = false;

    //initialize
    distance = 0.0;
    isValidDistance = false;
    mp_pimpl->mp_simplex->reset();
    mp_pimpl->mp_support->reset();
    VectorF searchDir = VectorFHelper::subtract_vector(shape2.centroid(), shape1.centroid()); //arbitrary search dir
    VectorF spp0;
    Support::ResultFlag result_flag = mp_pimpl->mp_support->support(&shape1, &shape2, searchDir, false, spp0);

//    qDebug() << "shape2.centroid() = " << shape2.centroid();
//    qDebug() << "shape1.centroid() = " << shape1.centroid();
//    qDebug() << "searchDir = " << searchDir;
//    qDebug() << "spp0 = " << spp0;


    isIntersect = result_flag == Support::ResultFlag::SUPPORT_ON_ORIGIN;
    if(result_flag == Support::ResultFlag::SUPPORT_BEYOND_ORIGIN){
        isIntersect = mp_pimpl->mp_simplex->update(spp0, searchDir);  //will call handle0D(v) in Simplex class. return searchDir as Origin - vertex[0]. bool return is ignored here.
        Q_ASSERT(!isIntersect);

//        qDebug() << "spp0 = " << spp0;
//        qDebug() << "searchDir = " << searchDir;
//        qDebug() << "isIntersect = " << isIntersect;

        //iteration
        int k = 0;
        while(k++ < max_iteration()){

            //get next support pt
            VectorF spp;
            Support::ResultFlag result_flag = mp_pimpl->mp_support->support(&shape1, &shape2, searchDir, true, spp);
            //qDebug() << "spp = " << spp;
            if(result_flag == Support::ResultFlag::SUPPORT_ON_ORIGIN){
                isIntersect = true;
                break;
            }
            else if(result_flag == Support::ResultFlag::SUPPORT_ON_SIMPLEX){
                distance = VectorFHelper::norm2(searchDir);
                isValidDistance = true;
                break;
            }

            //update simplex
            isIntersect =  mp_pimpl->mp_simplex->update(spp, searchDir); //note searchDir is updated with the next search direction to use
//            qDebug() << "searchDir = " << searchDir;
//            qDebug() << "isIntersect = " << isIntersect;
            if(isIntersect){
                break;
            }
        }

        //throws warning if max iteration was reached
        if(k > max_iteration() - 1){
            qWarning() << "[Gjk::chkIntersect] Maximum iteration reached while searching for origin in simplex. Results may not be accurate!";
        }
    } //if(result_flag == 1)
    return(isIntersect);
}

RRTPLANNER_FRAMEWORK_ALGORITHM_GJK_END_NAMESPACE
