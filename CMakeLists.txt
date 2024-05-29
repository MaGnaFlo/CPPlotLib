cmake_minimum_required(VERSION 3.16)

project(CPPlotLib VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python 3.12 COMPONENTS Development REQUIRED)
find_package(Qt6 6.2 COMPONENTS Quick Gui Widgets REQUIRED)
set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE MOC_SOURCES include/*.hpp)

set(PRE_BUILD_SCRIPT ${CMAKE_SOURCE_DIR}/pre_build.py)

add_executable(cpplotlib ${SRC_FILES} ${MOC_SOURCES})

add_custom_target(
        run_script ALL
        COMMAND python3 ${PRE_BUILD_SCRIPT}
        #  BYPRODUCTS enums.h
        COMMENT "Generating enums"
)

add_dependencies(cpplotlib run_script)

target_include_directories(cpplotlib
        PRIVATE
        ${Python_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cpplotlib
        PRIVATE
        ${Python_LIBRARIES}
        Qt6::Gui
        Qt6::Widgets
        Qt6::Quick)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS) # prevents clash between Qt slots and python.h
set_target_properties(cpplotlib PROPERTIES AUTOMOC TRUE)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(cpplotlib
        URI cpplotlib_ui
        VERSION 1.0
        QML_FILES qml/main.qml
)
