#include "WayptQTests.h"
#include "SegmentQTests.h"
#include "PlanQTests.h"
#include "LinearAlgebraHelperQTests.h"
#include <QtTest/QtTest>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    WayptQTests wayptQTests;
    SegmentQTests segmentQTests;
    PlanQTests planQTests;
    LinearAlgebraHelperQTests linearAlgebraHelperQTests;

    int status = QTest::qExec(&wayptQTests, argc, argv) + \
                 QTest::qExec(&segmentQTests, argc, argv) + \
                 QTest::qExec(&planQTests, argc, argv) + \
                 QTest::qExec(&linearAlgebraHelperQTests, argc, argv);
    return status;
}
