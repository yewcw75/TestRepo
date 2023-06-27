#ifndef RRTPLANNER_LIB_SEGMENTQTESTS_H
#define RRTPLANNER_LIB_SEGMENTQTESTS_H

#include <RrtPlannerLib/framework/Segment.h>
#include <QObject>
#include <QScopedPointer>

using namespace rrtplanner::framework;
class SegmentQTests : public QObject
{
    Q_OBJECT

public:
    SegmentQTests();
    ~SegmentQTests();

private:
    void setup();
    void cleanUp();
    void verify(const Segment& segment,
                int id,
                float northing_nodePrev,
                float easting_nodePrev,
                float northing_nodeNext,
                float easting_nodeNext,
                float northing_res_tVec,
                float easting_res_tVec,
                float northing_res_nVec,
                float easting_res_nVec,
                float segLength_res);

private slots:
    void verify_set_segment_data();
    void verify_set_segment();
    void verify_set_bisector();
    void verify_set_bisector_2();


};

#endif
