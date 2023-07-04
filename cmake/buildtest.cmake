
if(${ament_cmake_FOUND})
find_package(ament_lint_auto REQUIRED)
    # the following line skips the linter which checks for copyrights
    # comment the line when a copyright and license is added to all source files
    set(ament_cmake_copyright_FOUND TRUE)
    # the following line skips cpplint (only works in a git repo)
    # comment the line when this package is in a git repo and when
    # a copyright and license is added to all source files
    set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
endif()

enable_testing()

###QTest#####
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

add_executable(${PROJECT_NAME}QTests
    tests/main.cpp
    tests/framework/VectorFQTests.h
    tests/framework/VectorFQTests.cpp
    tests/framework/WayptQTests.h
    tests/framework/WayptQTests.cpp
    tests/framework/SegmentQTests.h
    tests/framework/SegmentQTests.cpp
    tests/framework/PlanQTests.h
    tests/framework/PlanQTests.cpp
    tests/framework/PlanHelperQTests.h
    tests/framework/PlanHelperQTests.cpp
    tests/framework/UblasHelperQTests.h
    tests/framework/UblasHelperQTests.cpp
    tests/framework/EllMapQTests.h
    tests/framework/EllMapQTests.cpp
    )

target_include_directories(${PROJECT_NAME}QTests PRIVATE
    ./
    ./incl/${PROJECT_NAME}/
    ./incl/${PROJECT_NAME}/framework
    ./incl/${PROJECT_NAME}/controllers
    ./incl/${PROJECT_NAME}/models
    ./pimpl/${PROJECT_NAME}/framework
    ./pimpl/${PROJECT_NAME}/controllers
    ./pimpl/${PROJECT_NAME}/models
    ./tests
    ./tests/framework
    ./tests/controllers
    ./tests/models
    ${Boost_INCLUDE_DIRS}
    )

if(${ament_cmake_FOUND})
target_link_libraries(
    ${PROJECT_NAME}QTests
    Qt${QT_VERSION_MAJOR}::Test
    ${PROJECT_NAME}
    )
else()
target_link_libraries(
    ${PROJECT_NAME}QTests
    Qt::Test
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Widgets
    ${PROJECT_NAME}
    )
endif()

add_test(NAME ${PROJECT_NAME}QTests COMMAND ${PROJECT_NAME}QTests)
