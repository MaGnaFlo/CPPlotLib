cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CPPlotLib VERSION 1.0 DESCRIPTION "CPPlotLib library")

include(GNUInstallDirs)

find_package(Python 3.12 COMPONENTS Development REQUIRED)

# library
add_library(CPPlotLib SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/figure.cpp)

# includes
target_include_directories(CPPlotLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cpplotlib>
    ${Python_INCLUDE_DIRS}
)

# Python library
target_link_libraries(CPPlotLib PRIVATE ${Python_LIBRARIES})

# library properties
set_target_properties(CPPlotLib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "include/cpplotlib/figure.hpp;include/cpplotlib/plot.hpp;include/cpplotlib/lineplot.hpp;include/cpplotlib/scatterplot.hpp;include/cpplotlib/barplot.hpp;include/cpplotlib/pieplot.hpp"
)

# install library, headers, and CMake config files
install(TARGETS CPPlotLib
        EXPORT CPPlotLibTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cpplotlib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/cpplotlib
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cpplotlib
    FILES_MATCHING PATTERN "*.hpp"
)

# install the exported targets file
install(EXPORT CPPlotLibTargets
        FILE CPPlotLibTargets.cmake
        NAMESPACE CPPlotLib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CPPlotLib)

# ConfigVersion file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CPPlotLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# main Config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/CPPlotLibConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CPPlotLib
)

# Install the Config and Version files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CPPlotLibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CPPlotLibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CPPlotLib
)
