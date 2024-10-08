cmake_minimum_required(VERSION 3.16)

project(cpplotlib_qml VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Gui Widgets REQUIRED)
find_package(CPPlotLib REQUIRED)
find_package(Python 3.12 COMPONENTS Development Interpreter REQUIRED)

# virtual environment modules
execute_process(COMMAND ${Python_EXECUTABLE} -m venv ${CMAKE_CURRENT_BINARY_DIR}/python_modules)
execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/python_modules/bin/pip install numpy matplotlib)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE MOC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_executable(cpplotlib_qml ${SRC_FILES} ${MOC_SOURCES})

target_include_directories(cpplotlib_qml
        PRIVATE
        ${Python_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(cpplotlib_qml
        PRIVATE
        ${Python_LIBRARIES}
        Qt6::Gui
        Qt6::Widgets
        Qt6::Quick
        CPPlotLib::CPPlotLib)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS) # prevents clash between Qt slots and python.h
set_target_properties(cpplotlib_qml PROPERTIES AUTOMOC TRUE)

qt_policy(SET QTP0001 NEW)

qt_add_qml_module(cpplotlib_qml
        URI cpplotlib_qml_ui
        VERSION 1.0
        QML_FILES qml/main.qml
)
