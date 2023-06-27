#include "WayptQTests.h"
#include "SegmentQTests.h"
#include "PlanQTests.h"
#include <QtTest/QtTest>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    WayptQTests wayptQTests;
    SegmentQTests segmentQTests;
    PlanQTests planQTests;

    int status = QTest::qExec(&wayptQTests, argc, argv) + \
                 QTest::qExec(&segmentQTests, argc, argv) + \
                 QTest::qExec(&planQTests, argc, argv);
    return status;
}
