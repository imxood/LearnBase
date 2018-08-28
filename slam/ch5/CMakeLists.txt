cmake_minimum_required(VERSION 2.8)
project(slam_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories("/usr/include/eigen3")

find_package(PCL REQUIRED COMPONENT common io)
include_directories(${PCL_INCLUDE_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(joinMap joinMap.cpp)
target_link_libraries(joinMap ${OpenCV_LIBS} ${PCL_LIBRARIES})

install(
    FILES pose.txt DESTINATION ${CMAKE_BINARY_DIR}
)

install(
    DIRECTORY color depth DESTINATION ${CMAKE_BINARY_DIR}
)