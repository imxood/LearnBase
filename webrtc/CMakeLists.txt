cmake_minimum_required(VERSION 2.8)
project(HelloTest)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_definitions(-DWEBRTC_POSIX)

find_package(PkgConfig REQUIRED)

pkg_check_modules(audio_processing audio_processing)
include_directories(${audio_processing_INCLUDE_DIRS})

# message(WARNING "audio_processing_CONFIG: ${audio_processing_CONFIG}")
message(WARNING "audio_processing_INCLUDE_DIRS: ${audio_processing_INCLUDE_DIRS}")
# message(WARNING "audio_processing_LIBRARIES:${audio_processing_LIBRARIES};${audio_processing_LIBRARY_DIRS};")

pkg_check_modules(webrtc webrtc)
include_directories(${webrtc_INCLUDE_DIRS})

message(WARNING "webrtc_CONFIG: ${webrtc_CONFIG}")
message(WARNING "webrtc_INCLUDE_DIRS: ${webrtc_INCLUDE_DIRS}")
message(WARNING "webrtc_LIBRARIES:${webrtc_LIBRARIES};${webrtc_LIBRARY_DIRS};")

add_executable(hello hello.cpp)
target_link_libraries(hello ${webrtc_LIBRARIES} pthread)