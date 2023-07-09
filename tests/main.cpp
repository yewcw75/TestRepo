#include "WayptQTests.h"
#include "SegmentQTests.h"
#include "PlanQTests.h"
#include "PlanHelperQTests.h"
#include "UblasHelperQTests.h"
#include "VectorFQTests.h"
#include "EllMapQTests.h"
#include "PolygonQTests.h"
#include <QtTest/QtTest>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    VectorFQTests vectorFQTests;
    WayptQTests wayptQTests;
    SegmentQTests segmentQTests;
    PlanQTests planQTests;
    PlanHelperQTests planHelperQTests;
    UblasHelperQTests linearAlgebraHelperQTests;
    EllMapQTests ellMapQTests;
    PolygonQTests polygonQTests;

    int status = \
            QTest::qExec(&vectorFQTests, argc, argv) + \
            QTest::qExec(&wayptQTests, argc, argv) + \
            QTest::qExec(&segmentQTests, argc, argv) + \
            QTest::qExec(&planQTests, argc, argv) + \
            QTest::qExec(&planHelperQTests, argc, argv) + \
            QTest::qExec(&linearAlgebraHelperQTests, argc, argv) + \
            QTest::qExec(&ellMapQTests, argc, argv) + \
            QTest::qExec(&polygonQTests, argc, argv);

    return status;
}
