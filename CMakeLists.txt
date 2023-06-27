cmake_minimum_required(VERSION 3.16)
project(RrtPlannerLib VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
message("CMAKE_MODULE_PATH (cmake script are located here): " ${CMAKE_MODULE_PATH})

message("CMAKE_CXX_COMPILER_ID = " ${CMAKE_CXX_COMPILER_ID})
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wno-c++98-compat)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wno-deprecated-declarations)
endif()

#############################
# find dependencies
find_package(ament_cmake QUIET)
message("ament_cmake_FOUND = " ${ament_cmake_FOUND})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network Widgets)
message("QT_VERSION_MAJOR = " ${QT_VERSION_MAJOR})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)
message("Boost_INCLUDE_DIRS = " ${Boost_INCLUDE_DIRS} )

#############################
# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
if(${QT_VERSION_MAJOR} STRLESS 6)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
else()
    qt_standard_project_setup()
endif()
include(GNUInstallDirs)

#############################
#add project files to our exe/lib
add_library(${PROJECT_NAME} SHARED
  incl/${PROJECT_NAME}/RrtPlannerLibGlobal.h
  incl/${PROJECT_NAME}/framework/FrameworkDefines.h
  incl/${PROJECT_NAME}/framework/GeometryHelper.h
  src/framework/GeometryHelper.cpp
  incl/${PROJECT_NAME}/framework/LinearAlgebraHelper.h
  src/framework/LinearAlgebraHelper.cpp
  incl/${PROJECT_NAME}/framework/Waypt.h
  src/framework/Waypt.cpp
  incl/${PROJECT_NAME}/framework/Segment.h
  src/framework/Segment.cpp
  incl/${PROJECT_NAME}/framework/Plan.h
  src/framework/Plan.cpp
  incl/${PROJECT_NAME}/framework/PlanHelper.h
  src/framework/PlanHelper.cpp
)

#############################
#include directories
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/incl>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>  # Relative install include path
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pimpl/framework>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pimpl/models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/pimpl/controllers>
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

#############################
#Preprocess definitions
target_compile_definitions(${PROJECT_NAME}
    PRIVATE RRTPLANNER_LIB_LIBRARY
)

#############################
#link dependencies
if(${ament_cmake_FOUND})
    ament_target_dependencies(${PROJECT_NAME}
        Qt${QT_VERSION_MAJOR}
        Qt${QT_VERSION_MAJOR}Core
        Qt${QT_VERSION_MAJOR}Gui
        Qt${QT_VERSION_MAJOR}Network
        Qt${QT_VERSION_MAJOR}Widgets
        Boost
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Network
        Qt::Widgets
    )
endif()

#############################
#installation
include(install) #run install.cmake located in CMAKE_MODULE_PATH (set on top)

#############################
#For ament_cmake
#When building a reusable library, some information needs to be exported
#for downstream packages to easily use it.
if(${ament_cmake_FOUND})
    ament_export_targets(${PROJECT_NAME}Targets HAS_LIBRARY_TARGET)

    #for ament_cmake to locate this lib's dependencies
    ament_export_dependencies(Qt${QT_VERSION_MAJOR} Boost)
endif()
#############################
#Deploy qt libs
#if(${QT_VERSION_MAJOR} STRLESS 6)
#    # Include Qt deployment helper function
#    include(windeployqt)
#    windeployqt(${PROJECT_NAME})
#else()
#    qt_generate_deploy_app_script(
#        TARGET ${PROJECT_NAME}
#        FILENAME_VARIABLE deploy_script
#        NO_UNSUPPORTED_PLATFORM_ERROR
#    )
#    install(SCRIPT ${deploy_script})
#endif()

#############################
message("BUILD_TESTING = " ${BUILD_TESTING})
if(BUILD_TESTING)
    include(buildtest)
endif()

#############################
if(${ament_cmake_FOUND})
    ament_package()
endif()

