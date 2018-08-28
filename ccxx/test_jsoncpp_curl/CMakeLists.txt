cmake_minimum_required(VERSION 3.1)
project(HelloTest)

set(CMAKE_BUILD_TYPE Debug)

message(WARNING "hello")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

ADD_DEFINITIONS(-DJSON_FILE_PATH="${CMAKE_SOURCE_DIR}/config/test.json")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

find_package(CURL REQUIRED)

# link_libraries(${JSONCPP_LIBRARIES})

# message("JSONCPP_CONFIG: ${JSONCPP_CONFIG}")
# message("JSONCPP_INCLUDE_DIRS: ${JSONCPP_INCLUDE_DIRS}")
# message("JSONCPP_LIBRARIES: ${JSONCPP_LIBRARIES} ${JSONCPP_LIBRARY}")

include_directories(${CURL_INCLUDE_DIR} ${JSONCPP_INCLUDE_DIRS} include)

add_executable(test_json src/test_json.cpp)
target_link_libraries(test_json ${JSONCPP_LIBRARIES})

add_executable(test_curl src/test_curl.cpp)
target_link_libraries(test_curl ${JSONCPP_LIBRARIES} ${CURL_LIBRARIES})
