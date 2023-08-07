#include <RrtPlannerLib/framework/PlanHelper.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>
#include <RrtPlannerLib/framework/VectorFHelper.h>
#include <RrtPlannerLib/framework/UblasHelper.h>
#include <QtGlobal>
#include <QDebug>
#include <limits>

namespace bnu = boost::numeric::ublas;

RRTPLANNER_FRAMEWORK_BEGIN_NAMESPACE

//----------
PlanHelper::PlanHelper()
{

}

//----------
PlanHelper::~PlanHelper()
{

}

//----------
PlanHelper::VerifyPlanResult PlanHelper::verifyPlanInput(const QVector<Waypt>& wayptList)
{
    VerifyPlanResult res = VerifyPlanResult::VERIFY_PLAN_OK;
    int nWaypt = wayptList.size();

    //check if there are more than 1 waypt
    res = nWaypt > 1 ? VerifyPlanResult::VERIFY_PLAN_OK : VerifyPlanResult::VERIFY_PLAN_ERR_SINGLE_WAYPT;

    //check no repeated point
    //check if any segment goes in a reverse direction
    if(res ==  VerifyPlanResult::VERIFY_PLAN_OK){   
        VectorF wayptPrev = wayptList.at(0).coord_const_ref();
        VectorF wayptLast = wayptList.at(nWaypt-1).coord_const_ref();
        VectorF vecFirstToLast = VectorFHelper::subtract_vector(wayptLast, wayptPrev);
        for(int idx = 1; idx < nWaypt; ++idx){ //loop from 2nd waypt onwards
            VectorF wayptNext = wayptList.at(idx).coord_const_ref();
            VectorF vecPrevToNext = VectorFHelper::subtract_vector(wayptNext, wayptPrev);
            //check repeat points
            double segLength = VectorFHelper::norm2(vecPrevToNext);
            if(segLength < TOL_SMALL){
                res = VerifyPlanResult::VERIFY_PLAN_ERR_REPEATED_WATPT;
                break;
            }
            //check reverse dir
            double dotPdt = VectorFHelper::dot_product(vecPrevToNext, vecFirstToLast);
            if(dotPdt < 0.0){
                res = VerifyPlanResult::VERIFY_PLAN_ERR_REVERSE_DIR;
                break;
            }
            wayptPrev = wayptNext; //for next iteration
        }
    }
    return(res);
}

//----------
bool PlanHelper::findNearestEdgeEvent(const Plan& plan,
                                     double crossTrackHorizon,
                                     double side,
                                     double eps_dx,
                                     double& dxNearest_out,
                                     QVector<int>& eventSegIdxList_out)
{
    bool found = false;
    QVector<int> eventSegIdxList;
    double dxNearest = std::numeric_limits<double>::max(); //init dx

    const QVector<Segment>& segmentList = plan.segmentList();
    int nSeg = segmentList.size();

    //edge event will need at least 2 segments
    if(nSeg > 1){
        //check event for each segment
        for(int i = 0; i < nSeg; ++i){
            const Segment& currSeg = segmentList.at(i);
            const VectorF& tVec = currSeg.tVec();
            const VectorF& nVec = currSeg.nVec();
            const VectorF& bVecPrev = currSeg.bVecPrev();
            const VectorF& bVecNext = currSeg.bVecNext();
            const VectorF& wayptPrev = currSeg.wayptPrev().coord_const_ref();
            const VectorF& wayptNext = currSeg.wayptNext().coord_const_ref();
            bnu::matrix M = UblasHelper::concatenate_col_vectors(bVecPrev.data_const_ref(),
                                                                         -1.0 * bVecNext.data_const_ref());
            bnu::vector<double> v = VectorFHelper::subtract_vector(wayptNext, wayptPrev).data_const_ref();
            bnu::vector<double> d;
            bool ok = UblasHelper::solve(M, v, TOL_SMALL, d);
            if(ok){
                bnu::vector<double> posEvent = wayptNext.data_const_ref() + d[1]*bVecNext.data_const_ref();
                bnu::matrix M2 = UblasHelper::concatenate_col_vectors(tVec.data_const_ref(),
                                                                              nVec.data_const_ref());
                bnu::vector<double> v2 = posEvent - wayptPrev.data_const_ref();
                bnu::vector<double> d2;
                bool ok2 = UblasHelper::solve(M2, v2, TOL_SMALL, d2);
                if(ok2){
                    double dx = side * d2[1]; //for port side, cast the problem to stbd side
                    if(dx > 0 && plan.crossTrack() + dx <= crossTrackHorizon){
                       if( dxNearest - dx >  eps_dx){ //=> dx is smaller than dxNearest
                           dxNearest = dx;
                           eventSegIdxList.clear();
                           eventSegIdxList.append(i);
                           found = true;
                       }
                       else if (abs(dxNearest - dx) <= eps_dx){  //=> curr event within close proximity of previous nearest
                           eventSegIdxList.append(i);
                       }
                    }
                } //if(ok2)
            } //if(ok)

        }
    } //if(nSeg > 1)
    dxNearest = side * dxNearest;

    if(found){ //assign output
        dxNearest_out = dxNearest;
        eventSegIdxList_out = eventSegIdxList;
    }
    return(found);
}

//----------
/**
 * @note Developer's note: In matlab version, k_event is used to keep track of the segment index found in eventSegIdxList
 * that we are now looking out for while processing the current input plan's segment.
 * Here, we simply check if the current segment index is contained in eventSegIdxList using QVector::contains() methos.
 * Not expected to be an intensive computation process since eventSegIdxList is not expected to be a super long list.
 *
 */
Plan PlanHelper::getCrossTrackPlan(const Plan& plan,
                                  double crossTrackHorizon,
                                  double dx,
                                  const QVector<int>& eventSegIdxList,
                                  double tol_small,
                                  bool* results_out,
                                  QString* results_desc)
{
    if(crossTrackHorizon < 0){
        crossTrackHorizon = abs(crossTrackHorizon);
        qWarning() << "[PlanHelper::getCrossTrackPlan] input crossTrackHorizon is negative but should be positive value. Using the absolute value.";
    }

    int nEventSeg = eventSegIdxList.size();
    const QVector<Segment>& segList = plan.segmentList();
    int nSeg = segList.size();

    Plan planOut; //plan to return
    planOut.setCrossTrack(plan.crossTrack() + dx); //set cross track first

    if(nEventSeg < nSeg) {//if no. of segments with events is smaller the number of seg => new plan is not just one point
        QVector<Waypt> wayptList_planOut;
        QVector<int> segIdList_planOut;

        //loop thru ref plan's segment and start building
        for (int idxSeg = 0; idxSeg < nSeg; ++idxSeg){  
            //if curr segment has event, dun proceed to form segment
            if(eventSegIdxList.contains(idxSeg)){
                continue;
            }

            const Segment& currSeg = segList.at(idxSeg);
            if(wayptList_planOut.isEmpty()){
                Waypt wayptPrev(currSeg.wayptPrev());
                wayptPrev.setCoord(findOffsetWaypt(currSeg.wayptPrev().coord_const_ref(), currSeg.nVec(), currSeg.bVecPrev(), dx, tol_small));
                wayptList_planOut.append(wayptPrev);
            }
            Waypt wayptNext(currSeg.wayptNext());
            wayptNext.setCoord(findOffsetWaypt(currSeg.wayptNext().coord_const_ref(), currSeg.nVec(), currSeg.bVecNext(), dx, tol_small));
            wayptList_planOut.append(wayptNext);
            segIdList_planOut.append(currSeg.id());
        } //for idxSeg = 1: nSeg

        //set planOut
        bool setOk = planOut.setPlan(wayptList_planOut, segIdList_planOut, results_desc);
        if(results_out){
            *results_out = setOk;
        }
        /*
        if(results_desc){
            qInfo() << "[PlanHelper::getCrossTrackPlan] set plan results: " << *results_desc;
        }
        */
        planOut.setProperty(Plan::Property::IS_LIMIT, abs(planOut.crossTrack()) - crossTrackHorizon > -TOL_SMALL);
    }
    else { //new plan is a point
        const Segment& firstSeg = segList.first();
        const Segment& lastSeg = segList.last();

        VectorF coord_offset = findOffsetWaypt(firstSeg.wayptPrev().coord_const_ref(), firstSeg.nVec(), firstSeg.bVecPrev(), dx, tol_small);
        Waypt wayptPrev(lastSeg.wayptPrev());
        wayptPrev.setCoord(coord_offset);
        Waypt wayptNext(lastSeg.wayptNext());
        wayptNext.setCoord(coord_offset);
        Segment newSeg(wayptPrev, wayptNext, lastSeg.id());
        newSeg.setSegmentAttributes(); //will set length
        newSeg.setLengthCumulative(newSeg.length());

        planOut.appendSegment(newSeg);
        planOut.setProperty(Plan::Property::IS_LIMIT);

        if(results_out){
            *results_out = true;
        }
        if(results_desc){
            *results_desc = QString("[PlanHelper::getCrossTrackPlan] Plan is a point. Set Ok.");
        }
    }
    return(planOut);
}

//----------
bool PlanHelper::buildSingleSideEllMap(const Plan& planNominal,
                                       double side,
                                       double crossTrackHorizon,
                                       QList<Plan>& planList,
                                       QString* results_desc)
{
    bool ret = true;
    int nSegNominal = planNominal.nSegment();

    //append nominal plan when side is stbd
    if(side > 0.0){
        planList.push_back(planNominal);
    }

    //while loop to build ellmap on one side
    Plan planRef(planNominal); //make a copy of nominal plan
    planRef.setProperty(Plan::Property::IS_NOMINAL, false);
    int countWhileLoop = 0; //additional check to prevent infinite while loop
    //qInfo() << planRef;
    while( planRef.nSegment() > 1 && //need min of 2 segs to have edge event
        countWhileLoop < nSegNominal //max possible no. of event is nSegNominal - 1
           )
    {
        double dxNearest_out{};
        QVector<int> eventSegIdxList_out;
        bool found = PlanHelper::findNearestEdgeEvent(planRef,
                                                      crossTrackHorizon,
                                                      side,
                                                      EPS_DX,
                                                      dxNearest_out,
                                                      eventSegIdxList_out);
        if(found){
            planRef = PlanHelper::getCrossTrackPlan(planRef,
                                        crossTrackHorizon,
                                        dxNearest_out,
                                        eventSegIdxList_out,
                                        TOL_SMALL,
                                        &ret, //false if error
                                        results_desc //results description
                                        );
            if(!ret){
                break; //break while loop
            }
            //qInfo() << planRef;
            Plan plan2Append(planRef);
            insertDummySegments(plan2Append, nSegNominal);
            pushPlan(plan2Append, side, planList);//push to plan list
        }
        else{ //no edge event found
            break;
        }
        ++countWhileLoop;
    } //while-loop

    //max cross-track plan
    if(ret){ //if no error so far
        if(!planRef.testProperty(Plan::Property::IS_LIMIT)){ //if last plan was not the limit
            planRef = PlanHelper::getCrossTrackPlan(planRef,
                                                    crossTrackHorizon,
                                                    side*crossTrackHorizon - planRef.crossTrack(),
                                                    QVector<int>(),
                                                    TOL_SMALL,
                                                    &ret, //false if error
                                                    results_desc //results description
                                                    );
            planRef.setProperty(Plan::Property::IS_LIMIT);
            Plan plan2Append(planRef);
            insertDummySegments(plan2Append, nSegNominal);
            pushPlan(plan2Append, side, planList);//push to plan list
        }
    }
    return(ret);
}

//----------
void PlanHelper::insertDummySegments(Plan& plan, int nSegNominal)
{
    const QVector<Segment>& segList = plan.segmentList();
    int nSeg = segList.size();
    assert(nSeg > 0);
    QVector<Segment> segListOut;

    int scount = 0;
    double lengthCumulative = 0.0;
    for (int idNominal = 0; idNominal < nSegNominal; ++idNominal){
       Segment seg2Insert = segList.at(scount);
       if (idNominal < segList.at(scount).id()){
           seg2Insert.setId(idNominal);
           seg2Insert.setWayptNext(seg2Insert.wayptPrev());
           seg2Insert.setbVecNext(seg2Insert.bVecPrev());
           seg2Insert.setLength(0.0);
       }
       else if (idNominal > segList.at(scount).id()){
           seg2Insert.setId(idNominal);
           seg2Insert.setWayptPrev(seg2Insert.wayptNext());
           seg2Insert.setbVecPrev(seg2Insert.bVecNext());
           seg2Insert.setLength(0.0);
       }
       else { //=> segList.at(scount).id == idNominal
           ++scount;
           scount = scount > nSeg - 1? nSeg - 1: scount; //clip to nSeg - 1
        }
       lengthCumulative += seg2Insert.length();
       seg2Insert.setLengthCumulative(lengthCumulative);
       segListOut.push_back(seg2Insert); //append segment to segListOut
    } //for-loop
    plan.setSegmentList(segListOut); //replace the segment list in plan
}

//----------
void PlanHelper::pushPlan( const Plan& plan,
                            double side,
                            QList<Plan>& planList //planList to prepend/append
                         )
{
    if(side < 0){
        planList.push_front(plan);
    }
    else
    {
        planList.push_back(plan);
    }
}

//----------
VectorF PlanHelper::findOffsetWaypt(const VectorF& pt,
                                    const VectorF& nVec,
                                    const VectorF& bVec,
                                    double dx, //crosstrack to offset
                                    double tol_small
                                    )
{
    //solve scalar, a, using relation: dot(a.bvec, nvec) = dx => a = dx/dot(bvec, nvec)
    double dotPdt = VectorFHelper::dot_product(bVec, nVec);
    bool ok = abs(dotPdt) >= tol_small;
    if(!ok){
        qFatal("[PlanHelper::findOffsetWaypt] Division by zero error");
    }
    double  a = dx/dotPdt;
    VectorF res = VectorFHelper::add_vector(pt, VectorFHelper::multiply_value(bVec, a)); //pt + a * bVec
    return(res);
}

//----------
RRTPLANNER_FRAMEWORK_END_NAMESPACE
