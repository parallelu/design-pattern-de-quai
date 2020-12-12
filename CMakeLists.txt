cmake_minimum_required(VERSION 3.12)
project(design_pattern_de_quai)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.75.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES s2_solid_design_principles/journal.cpp)
add_executable(design_pattern_de_quai ${SOURCE_FILES})

target_link_libraries(design_pattern_de_quai ${Boost_LIBRARIES})

