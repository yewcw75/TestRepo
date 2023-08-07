#include <RrtPlannerLib/framework/UtilHelper.h>
#include <RrtPlannerLib/framework/EllMap.h>
#include <RrtPlannerLib/framework/SPlan.h>
#include <RrtPlannerLib/framework/SMap.h>
#include <RrtPlannerLib/framework/FrameworkDefines.h>

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
bool UtilHelper::compare(const EllMap& d1, const EllMap& d2, double tol_small)
{
    bool ret = d1.size() == d2.size();
    if(ret){
        for(int i = 0; i < d1.size(); ++i){
            bool idSame = d1.at(i).id() == d2.at(i).id();
            bool nSegSame = d1.at(i).nSegment() == d2.at(i).nSegment();
            bool crossTrackSame = UtilHelper::compare(d1.at(i).crossTrack(), d2.at(i).crossTrack(), tol_small);
            bool lengthSame = UtilHelper::compare(d1.at(i).length(), d2.at(i).length());
            if(!(idSame && nSegSame && crossTrackSame && lengthSame)){
                ret = false;
                break;
            }
            bool segSame = true;
            for(int j = 0; j < d1.at(i).nSegment(); ++j){
                const Segment& seg = d1.at(i).segmentList().at(j);
                const Segment& seg_expected = d2.at(i).segmentList().at(j);
                bool wayptPrevEastingSame = UtilHelper::compare(seg.wayptPrev().easting(), seg_expected.wayptPrev().easting());
                bool wayptPrevNorthingSame = UtilHelper::compare(seg.wayptPrev().northing(), seg_expected.wayptPrev().northing());
                bool wayptNextEastingSame = UtilHelper::compare(seg.wayptNext().easting(), seg_expected.wayptNext().easting());
                bool wayptNextNorthingSame = UtilHelper::compare(seg.wayptNext().northing(), seg_expected.wayptNext().northing());
                bool tnVecSame = true;
                if(seg.length()> tol_small){
                    bool tVecNorthingSame  = UtilHelper::compare(seg.tVec().at(IDX_NORTHING), seg_expected.tVec().at(IDX_NORTHING));
                    bool tVecEastingSame  = UtilHelper::compare(seg.tVec().at(IDX_EASTING), seg_expected.tVec().at(IDX_EASTING));
                    bool nVecNorthingSame  = UtilHelper::compare(seg.nVec().at(IDX_NORTHING), seg_expected.nVec().at(IDX_NORTHING));
                    bool nVecEastingSame  = UtilHelper::compare(seg.nVec().at(IDX_EASTING), seg_expected.nVec().at(IDX_EASTING));
                    tnVecSame = tVecNorthingSame && tVecEastingSame && nVecNorthingSame && nVecEastingSame;
                }
                bool segLengthSame = UtilHelper::compare(seg.length(), seg_expected.length());
                bool segCumLengthSame = UtilHelper::compare(seg.lengthCumulative(), seg_expected.lengthCumulative());
                segSame = wayptPrevEastingSame && wayptPrevNorthingSame && wayptNextEastingSame && wayptNextNorthingSame && \
                      tnVecSame &&  segLengthSame && segCumLengthSame;
                if(!segSame){
                    break;
                }
            } //for j
            if(!segSame){
                ret  = false;
                break;
            }
        } //for i
    }
    return(ret);
}

//----------
bool UtilHelper::compare(const SPlan& d1, const SPlan& d2, double tol_small)
{
    bool crossTrackSame = compare(d1.getCrosstrack(), d2.getCrosstrack(), tol_small);
    bool lhSame = compare(d1.getLh(), d2.getLh(), tol_small);
    bool volSame = compare(d1.getVol_cum(), d2.getVol_cum(), tol_small);
    bool areaSame = compare(d1.getArea_cum(), d2.getArea_cum(), tol_small);
    return crossTrackSame && lhSame && volSame && areaSame;
}

//----------
bool UtilHelper::compare(const SMap& d1, const SMap& d2, double tol_small)
{
    bool sizeSame = d1.size() == d2.size();
    bool idxNominalSame = d1.idxNominal() == d2.idxNominal();
    bool lhSame = d1.lh0() == d2.lh0();
    bool thSame = d1.th0() == d2.th0();
    bool umaxSame = d1.umax() == d2.umax();
    bool uminSame = d1.umin() == d2.umin();
    bool ret = sizeSame && idxNominalSame && lhSame && thSame && umaxSame && uminSame;
    if(ret)
    {
        for(int i = 0; i < d1.size(); ++i)
        {
            if(!compare(d1.at(i), d2.at(i), tol_small)){
                ret = false;
                break;
            }
        }
    }
    return(ret);
}

//----------
int UtilHelper::mod(int x, int N)
{
    return((x % N + N) % N);
}

RRTPLANNER_FRAMEWORK_END_NAMESPACE
