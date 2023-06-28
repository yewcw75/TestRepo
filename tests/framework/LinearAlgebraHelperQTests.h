#ifndef RRTPLANNER_LIB_LINEARALGEBRAQTESTS_H
#define RRTPLANNER_LIB_LINEARALGEBRAQTESTS_H

#include <RrtPlannerLib/framework/LinearAlgebraHelper.h>
#include <QObject>

using namespace rrtplanner::framework;
class LinearAlgebraHelperQTests : public QObject
{
    Q_OBJECT

public:
    LinearAlgebraHelperQTests();
    ~LinearAlgebraHelperQTests();

private slots:
    void verify_solve_data();
    void verify_solve();
    void verify_to_bnu_vector();
    void verify_to_bgm_vector();
    void verify_concatenate_col_vectors();
};

#endif
