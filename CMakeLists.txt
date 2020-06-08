cmake_minimum_required(VERSION 3.11.3)
project(memento VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

find_package(Boost COMPONENTS filesystem system REQUIRED)

file(GLOB SOURCES src/*.cpp)
add_executable(memento ${SOURCES})
target_include_directories(memento PRIVATE include/ ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(memento ${wxWidgets_LIBRARIES})
target_link_libraries(memento Boost::filesystem)

