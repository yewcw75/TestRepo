#ifndef RRTPLANNER_LIB_SEGMENTQTESTS_H
#define RRTPLANNER_LIB_SEGMENTQTESTS_H

#include <QObject>
#include <QScopedPointer>
#include <RrtPlannerLib/framework/Segment.h>

class SegmentQTests : public QObject
{
    Q_OBJECT

public:
    SegmentQTests();
    ~SegmentQTests();

private:
    void setup();
    void cleanUp();
    void verify(const rrtplanner::framework::Segment& segment,
                int id,
                double northing_wayptPrev,
                double easting_wayptPrev,
                double northing_wayptNext,
                double easting_wayptNext,
                double northing_res_tVec,
                double easting_res_tVec,
                double northing_res_nVec,
                double easting_res_nVec,
                double segLength_res);

private slots:
    void verify_set_segment_data();
    void verify_set_segment();
    void verify_set_bisector();
    void verify_set_bisector_2();


};

#endif
