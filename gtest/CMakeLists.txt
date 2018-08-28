cmake_minimum_required(VERSION 3.1)
project(HelloTest)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtest gtest)
# find_package(gtest REQUIRED)

message(WARNING "gtest_CONFIG:${gtest_CONFIG};")
message(WARNING "gtest_INCLUDE_DIRS:${gtest_INCLUDE_DIRS};")
message(WARNING "gtest_LIBRARIES:${gtest_LIBRARY_DIRS};${gtest_LIBRARIES};")

include_directories(${gtest_INCLUDE_DIRS})
# link_libraries(${gtest_LIBRARY_DIRS})

add_executable(hello Configure.cpp ConfigureTest.cpp Main.cpp)
target_link_libraries(hello ${gtest_LIBRARIES} pthread)
