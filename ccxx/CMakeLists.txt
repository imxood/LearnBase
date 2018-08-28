cmake_minimum_required(VERSION 3.1)
project(Test)

# AUX_SOURCE_DIRECTORY(. SRCS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE debug)

find_package(cpprestsdk REQUIRED)
include_directories(cpprestsdk_INCLUDE_DIRS)

message("cpprestsdk_INCLUDE_DIRS: '${cpprestsdk_INCLUDE_DIRS}'")

find_package(PkgConfig REQUIRED)
pkg_check_modules(utils utils)
message("utils: '${utils_INCLUDE_DIRS},${utils_LIBDIR}'")
link_directories(${utils_LIBDIR})
include_directories(${utils_INCLUDE_DIRS})

find_package(OpenSSL REQUIRED)
message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

include_directories(${OPENSSL_INCLUDE_DIR})
list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})

# pkg_check_modules(LOGGING easyloggingpp)
# include_directories(LOGGING_INCLUDE_DIRS)

pkg_check_modules(gtest gtest)
include_directories(${gtest_INCLUDE_DIRS})
# gtest_LIBRARIES

# include_directories(${Boost_INCLUDE_DIRS})
# add_executable(boost_process_ boost_process.cpp)
# target_link_libraries(boost_process_ boost_system pthread)

find_package(CURL REQUIRED)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS filesystem thread timer)
# add_executable(progname file1.cxx file2.cxx)
# target_link_libraries(progname ${Boost_LIBRARIES})

message("Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

find_package(RapidJSON REQUIRED)

find_package(glog REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${RapidJSON_INCLUDE_DIRS})

## 测试popen方法，屏蔽输出
add_executable(popen popen.cpp)

add_executable(_test test.cpp)
target_link_libraries(_test utils)

# add_executable(easylogging easylogging.cpp)
# target_link_libraries(easylogging easyloggingpp)

add_executable(cpprestsdk cpprestsdk.cpp)
target_link_libraries(cpprestsdk PRIVATE cpprestsdk::cpprest boost_system utils)

add_executable(lambda lambda.cpp)

add_executable(BaiduAPI BaiduAPI.cpp)
target_link_libraries(BaiduAPI boost_system cpprestsdk::cpprest utils)

add_executable(fstream fstream.cpp)
target_link_libraries(fstream boost_system utils)

add_executable(shared_memory_1 interprocess/shared_memory_1.cpp)
target_link_libraries(shared_memory_1 Boost::thread rt pthread)

add_executable(shared_memory_2 interprocess/shared_memory_2.cpp)
target_link_libraries(shared_memory_2 Boost::thread rt pthread)

add_executable(anonymous_condition_1 anonymous_condition_1.cpp)
target_link_libraries(anonymous_condition_1 rt pthread)

add_executable(anonymous_condition_2 anonymous_condition_2.cpp)
target_link_libraries(anonymous_condition_2 rt pthread)

add_executable(rapidjson1 rapidjson1.cpp)
target_link_libraries(rapidjson1 utils)

add_executable(rapidjson2 rapidjson2.cpp)
target_link_libraries(rapidjson2 utils)

add_executable(rapidjson3 rapidjson3.cpp)
target_link_libraries(rapidjson3 utils)

add_executable(managed_shared_memory managed_shared_memory.cpp)
target_link_libraries(managed_shared_memory rt pthread)

add_executable(task task.cpp)
target_link_libraries(task pthread rt cpprestsdk::cpprest utils)

add_executable(glog glog.cpp)
target_link_libraries(glog glog::glog)

add_executable(thread thread.cpp)
target_link_libraries(thread pthread rt utils)

add_executable(thread1 thread1.cpp)
target_link_libraries(thread1 pthread rt)

add_executable(shared_ptr shared_ptr.cpp)
target_link_libraries(shared_ptr utils)

add_executable(mutex mutex.cpp)
target_link_libraries(mutex utils pthread)

# add_executable(ace ace.cpp)
# target_link_libraries(ace utils pthread)

add_executable(boost_timer boost_timer.cpp)
target_link_libraries(boost_timer utils Boost::timer)

add_executable(boost_timer1 boost_timer1.cpp)
target_link_libraries(boost_timer1 utils Boost::timer)

add_executable(boost_timer2 boost_timer2.cpp)
target_link_libraries(boost_timer2 utils Boost::thread Boost::timer)

add_executable(openssl openssl.cpp)
target_link_libraries(openssl ${OPENSSL_LIBRARIES} utils)

add_executable(XfyunASR XfyunASR.cpp)
target_link_libraries(XfyunASR ${OPENSSL_LIBRARIES} cpprestsdk::cpprest utils Boost::system)

add_executable(alsa_rawportlist alsa_rawportlist.boot.cpp)
target_link_libraries(alsa_rawportlist asound utils)

add_executable(alsa_devices alsa_devices.cpp)
target_link_libraries(alsa_devices asound utils)

add_executable(alsa_play alsa_play.cpp)
target_link_libraries(alsa_play asound utils)

# add_executable(timewheel timewheel/timermanager.cc timewheel/threadtimer.cc timewheel/main.cc)
# target_link_libraries(timewheel utils)
