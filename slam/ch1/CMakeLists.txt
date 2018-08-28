cmake_minimum_required(VERSION 2.8)

project(hello)

#设置编译类型, 默认就是Release
if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

#设置头文件路径
include_directories(include)

#添加静态库
add_library(hello src/hello.cpp)

#添加可执行程序
add_executable(sayHello src/useHello.cpp)
target_link_libraries(sayHello hello)


install(
	TARGETS hello
	ARCHIVE DESTINATION lib　#安装静态库到/usr/local/lib目录
)

install(
	#安装头文件到/usr/local/include目录
	FILES ${PROJECT_SOURCE_DIR}/include/hello.h DESTINATION include
)
