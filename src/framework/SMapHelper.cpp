#include <RrtPlannerLib/framework/SMapHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <QtGlobal>
#include <QDebug>


RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//----------
SMapHelper::SMapHelper()
{

}

//----------
SMapHelper::~SMapHelper()
{

}

//----------
SMap SMapHelper::create(const EllMap& ellMap,
            const RootData& root_data,
            double lh0,
            double th0,
            double umin,
            double umax
            )
{
    //qInfo() << "[SMapHelper::create] root_data:" << root_data;

    SMap sMap;

    const QVector<double>& ellList = root_data.ell_list_const_ref();

    //first plan
    Plan planPrev = *ellMap.at(0);
    SPlan sPlan;
    sPlan.setCrosstrack(planPrev.crossTrack());
    double lh{}, ellMaxPrev{};
    determineArcLengthHorizon(planPrev, ellList.at(0), lh0, lh, ellMaxPrev);
    sPlan.setLh(lh);
    sMap.append(sPlan);
    if(planPrev.testProperty(Plan::Property::IS_NOMINAL)){
        sMap.setIdxNominal(0);
    }

    //Subsequent plans
    for (int idx_p = 1; idx_p < ellMap.nPlan(); ++idx_p){
        double lhNext{}, ellMaxNext{};
        determineArcLengthHorizon(*ellMap.at(idx_p), ellList.at(idx_p), lh0, lhNext, ellMaxNext);

        Plan planNext = *ellMap.at(idx_p);
        appendSPlans(planPrev, ellMaxPrev,
                     planNext, lhNext, ellMaxNext,
                     lh0, th0, umin, umax, sMap);

        if(planNext.testProperty(Plan::Property::IS_NOMINAL)){
            sMap.setIdxNominal(sMap.size() - 1);
        }

        //assign for next iter
        planPrev = planNext;
        ellMaxPrev = ellMaxNext;
    }

    //normalize volumes and areas
    double tot_vol = sMap.last().getVol_cum();
    double tot_area = sMap.last().getArea_cum();
    sMap.last().setVol_cum(1.0);
    sMap.last().setArea_cum(1.0);
    for (int np = 1; np < sMap.size() - 1; ++np){
        double vol_cum_curr = sMap.at(np).getVol_cum();
        sMap[np].setVol_cum(vol_cum_curr/tot_vol);

        double area_cum_curr = sMap.at(np).getArea_cum();
        sMap[np].setArea_cum(area_cum_curr/tot_area);
    }
    return sMap;
}

//----------
/**
 * @see matlab version: function [sMap] = DetermineSPlans(sMap,planPrev,aux_ellmaxPrev,planNext,aux_Lh,aux_ellmax,Lh,Th,Umin,Umax)
 */
void SMapHelper::appendSPlans(const Plan& planPrev, double ellmaxPrev,
                  const Plan& planNext, double lhNext, double ellmaxNext,
                  double lh0, double th0, double umin, double umax, SMap& sMap)
{
    //check whether arclength horizon is limited by final arclength at an intermediate crosstrack
    bool isLimitedOnPrevSide = ellmaxPrev < planPrev.length()  && ellmaxNext > planNext.length();
    bool isLimitedOnNextSide = ellmaxPrev > planPrev.length()  && ellmaxNext < planNext.length();
    if (isLimitedOnPrevSide || isLimitedOnNextSide)
    {
        //=== Arclength horizon capped at intermediate crosstrack ==============
        //crosstrack intersection
        double dx_prev2Next = planNext.crossTrack() - planPrev.crossTrack();
        double buff_prev = planPrev.length() - ellmaxPrev;
        double buff_next = planNext.length() - ellmaxNext;
        double int_dx = planPrev.crossTrack() + buff_prev/(buff_prev-buff_next)*dx_prev2Next;

        double x_vec[2] = {planPrev.crossTrack(), int_dx};
        double lh_vec[2] = {sMap.last().getLh(), lh0};
        appendSPlan(x_vec, lh_vec, th0, umin, umax, sMap);

        double x_vec_B[2] = {int_dx, planNext.crossTrack()};
        double lh_vec_B[2] = {lh0, lhNext};
        appendSPlan(x_vec_B, lh_vec_B, th0, umin, umax, sMap);
    }
    else
    {
        //=== Arclength horizon NOT capped at intermediate crosstrack ==========
        double x_vec[2] = {planPrev.crossTrack(), planNext.crossTrack()};
        double lh_vec[2] = {sMap.last().getLh(), lhNext};
        appendSPlan(x_vec, lh_vec, th0, umin, umax, sMap);
    }
}

//----------
/**
 * @see matlab version: function [sMap] = DetermineSPlans(sMap,planPrev,aux_ellmaxPrev,planNext,aux_Lh,aux_ellmax,Lh,Th,Umin,Umax)
 */
void SMapHelper::appendSPlan(const double x_vec[2], const double lh_vec[2], double th0, double umin, double umax, SMap& sMap)
{
    double vol_cum = sMap.last().getVol_cum();
    double area_cum = sMap.last().getArea_cum();
    bool timeHorizonLimited_prev = lh_vec[0] <= th0*umin; //at min speed, time horizon is limited by arc length horizon
    bool timeHorizonLimited_next = lh_vec[1] <= th0*umin;

    if ( timeHorizonLimited_prev && timeHorizonLimited_next && umin > 0 ){
        SPlan splan2append;
        splan2append.setCrosstrack(x_vec[1]);
        splan2append.setLh(lh_vec[1]);
        double vol = sampling_vol3(x_vec, lh_vec, x_vec, umin, umax);
        vol_cum += vol;
        splan2append.setVol_cum(vol_cum);
        double area = 0.5 * ( lh_vec[0] + lh_vec[1] ) * ( x_vec[1] - x_vec[0] );
        area_cum += area;
        splan2append.setArea_cum(area_cum);
        sMap.append(splan2append);
    }
    else if ( !timeHorizonLimited_prev && timeHorizonLimited_next ){
        Q_ASSERT(abs(lh_vec[1] - lh_vec[0]) > TOL_SMALL);

        double intU_dx = x_vec[0] + ((th0*umin-lh_vec[0])/(lh_vec[1]-lh_vec[0]))*(x_vec[1]-x_vec[0]);

        SPlan splan2append;
        splan2append.setCrosstrack(intU_dx);
        splan2append.setLh(th0*umin);
        double x_lim[2] = {x_vec[0], intU_dx}; // Initialize x_lim with values from x_vec[0] and intU_dx
        double vol = sampling_vol4( x_vec, lh_vec, x_lim, th0, umin, umax );
        vol_cum += vol;
        splan2append.setVol_cum(vol_cum);
        double area = 0.5* ( lh_vec[0] + th0*umin ) * ( intU_dx - x_vec[0] );
        area_cum += area;
        splan2append.setArea_cum(area_cum);
        sMap.append(splan2append);


        SPlan splan2append_B;
        splan2append_B.setCrosstrack(x_vec[1]);
        splan2append_B.setLh(lh_vec[1]);
        double x_lim_B[2] = {intU_dx, x_vec[1]};
        double vol_B = sampling_vol3( x_vec, lh_vec, x_lim_B, umin, umax );
        vol_cum += vol_B;
        splan2append_B.setVol_cum(vol_cum);
        double area_B = 0.5 * ( th0*umin + lh_vec[1] ) * ( x_vec[1] - intU_dx );
        area_cum += area_B;
        splan2append_B.setArea_cum(area_cum);
        sMap.append(splan2append_B);
    }
    else if ( timeHorizonLimited_prev && !timeHorizonLimited_next ){
        Q_ASSERT(abs(lh_vec[1] - lh_vec[0]) > TOL_SMALL);
        double intU_dx = x_vec[0] + ((th0*umin-lh_vec[0])/(lh_vec[1]-lh_vec[0]))*(x_vec[1]-x_vec[0]);

        SPlan splan2append;
        splan2append.setCrosstrack(intU_dx);
        splan2append.setLh(th0*umin);
        double x_lim[2] = {x_vec[0], intU_dx};
        double vol = sampling_vol3( x_vec, lh_vec, x_lim, umin, umax );
        vol_cum += vol;
        splan2append.setVol_cum(vol_cum);
        double area =0.5* ( lh_vec[0] + th0*umin ) * ( intU_dx - x_vec[0] );
        area_cum += area;
        splan2append.setArea_cum(area_cum);
        sMap.append(splan2append);

        SPlan splan2append_B;
        splan2append_B.setCrosstrack(x_vec[1]);
        splan2append_B.setLh(lh_vec[1]);
        double x_lim_B[2] = {intU_dx, x_vec[1]}; // Initialize x_lim with values from x_vec[0] and intU_dx
        double vol_B = sampling_vol4( x_vec, lh_vec, x_lim_B, th0, umin, umax );
        vol_cum += vol_B;
        splan2append_B.setVol_cum(vol_cum);
        double area_B = 0.5* ( th0*umin + lh_vec[1] ) * ( x_vec[1] - intU_dx );
        area_cum += area_B;
        splan2append_B.setArea_cum(area_cum);
        sMap.append(splan2append_B);
    }
    else{
        SPlan splan2append;
        splan2append.setCrosstrack(x_vec[1]);
        splan2append.setLh(lh_vec[1]);
        double vol = sampling_vol4( x_vec, lh_vec, x_vec, th0, umin, umax );
        vol_cum += vol;
        splan2append.setVol_cum(vol_cum);
        double area = 0.5 * ( lh_vec[0] + lh_vec[1] ) * ( x_vec[1] - x_vec[0] );
        area_cum += area;
        splan2append.setArea_cum(area_cum);
        sMap.append(splan2append);
    }

}

//----------
void SMapHelper::determineArcLengthHorizon(const Plan& plan,
                                   double ell0,
                                   double lH0,
                                   double& lh,
                                   double& ellMax
                                   )
{
    ellMax = ell0 + lH0;
    lh = ellMax > plan.length()? plan.length() : ellMax; //cap at plan length
    lh -= ell0; //we want the arclength horizon from ell0
    lh = lh < 0.0 ? 0.0 : lh; //cap at 0

//    qInfo() << "[SMapHelper::determineArcLengthHorizon] ell0 = " << ell0 << ", lH0 = " << lH0 << \
//               ", ellMax = "  << ellMax << ", plan.length() = " << plan.length();
//    qInfo() << "[SMapHelper::determineArcLengthHorizon] plan:" << plan;

}

//----------
double SMapHelper::sampling_vol3(const double x_vec[2],const double lh_vec[2], const double x_lim[2], double umin, double umax)
{
    double vol{}; //for return

    //Error checks
    bool errCond_1 = x_lim[0]<x_vec[0] || x_lim[0]>x_vec[1];
    bool errCond_2 = x_lim[1]<x_vec[0] || x_lim[1]>x_vec[1];
    bool errCond_3 = umin < TOL_SMALL || umax < TOL_SMALL;
    if (errCond_1 || errCond_2 || errCond_3){
        qCritical() << "[SMapHelper::sampling_vol3] input error! errCond_1 = " << errCond_1 << \
                       ", errCond_2 = " << errCond_2 << \
                       ", errCond_3 = " << errCond_3;
        Q_ASSERT(0);
    }

    //Compute volume
    if (x_vec[0]<x_vec[1]){

        // compute slope and ordinate of arclength horizon ------------------
        double m = (lh_vec[1]-lh_vec[0])/(x_vec[1]-x_vec[0]);
        double m_sq = m*m;
        double n = lh_vec[0];
        double n_sq = n*n;

        //compute volume ---------------------------------------------------
        double tmp = x_lim[1]-x_vec[0];
        double tmp_sq = tmp * tmp;
        double tmp2 = x_lim[0]-x_vec[0];
        double tmp2_sq = tmp2 * tmp2;

        double C = (1.0/6.0)*(1.0/umin-1.0/umax);
        double T1 = (m_sq*tmp_sq + 3.0*m*n*tmp + 3.0*n_sq)*tmp;
        double T2 = (m_sq*tmp2_sq + 3.0*m*n*tmp2 + 3*n_sq)*tmp2;
        vol = C*(T1 - T2);
//        qInfo() << "m = " << m << ", n = " << n << ", tmp = " << tmp << ", tmp_sq = " << tmp_sq << \
//                   ", tmp2 = " << tmp2 << ", tmp2_sq = " << tmp2_sq << ", C = " << C << ", T1 = " << T1 << \
//                   ", T2 = " << T2;
//        qInfo() << "vol = " << vol;
    }
    return(vol);
}

//----------
double SMapHelper::sampling_vol4(const double x_vec[2], const double lh_vec[2], const double x_lim[2], double th, double umin, double umax)
{
    double vol{}; //for return

    //Error checks
    bool errCond_1 = x_lim[0]<x_vec[0] || x_lim[0]>x_vec[1];
    bool errCond_2 = x_lim[1]<x_vec[0] || x_lim[1]>x_vec[1];
    bool errCond_3 = umin < TOL_SMALL || umax < TOL_SMALL;
    if (errCond_1 || errCond_2 || errCond_3){
        qCritical() << "[SMapHelper::sampling_vol4] input error! errCond_1 = " << errCond_1 << \
                       ", errCond_2 = " << errCond_2 << \
                       ", errCond_3 = " << errCond_3;
        Q_ASSERT(0);
    }

    //Compute volume
    if (x_vec[0]<x_vec[1]){
        //--- compute slope and ordinate of arclength horizon ------------------
        double m = (lh_vec[1]-lh_vec[0])/(x_vec[1]-x_vec[0]);
        double m_sq = m*m;
        double n = lh_vec[0];
        double n_sq = n*n;

        //--- compute volume ---------------------------------------------------
        double tmp = x_lim[1]-x_vec[0];
        double tmp_sq = tmp * tmp;
        double tmp2 = x_lim[0]-x_vec[0];
        double tmp2_sq = tmp2 * tmp2;
        double tmp3 = x_lim[1]-x_lim[0];

        long double T1 = th * ( 0.5*m*(x_lim[1]-2.0*x_vec[0]+x_lim[0]) + n - 0.5*th*umin )*tmp3;
        long double C = (1.0/6.0)*(1.0/umax);
        long double T2 = (m_sq*tmp_sq + 3.0*m*n*tmp + 3.0*n_sq)*tmp;
        long double T3 = (m_sq*tmp2_sq + 3.0*m*n*tmp2 + 3*n_sq)*tmp2;
        vol = T1 - C*(T2 - T3);

//        qInfo() << "m = " << m << ", n = " << n << ", tmp = " << tmp << ", tmp_sq = " << tmp_sq << \
//                   ", tmp2 = " << tmp2 << ", tmp2_sq = " << tmp2_sq << ", tmp3 = " << tmp3 << ", C = " << (double)C << ", T1 = " << (double)T1 << \
//                   ", T2 = " << (double)T2 << ", T3 = " << (double)T3;
//        qInfo() << "vol = " << vol;
       }
    return(vol);
}



//----------
RRTPLANNER_FRAMEWORK_END_NAMESPACE
