cmake_minimum_required(VERSION 3.1)
project(Test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(var "ABC")

macro(Moo arg)
    message("arg = ${arg}")
    set(arg "abc")
    message("# After change the value of arg.")
    message("arg = ${arg}")
endmacro()

message("=== Call macro ===")
Moo(${var})

message("arg = ${arg}")

function(Foo arg)
    message("arg = ${arg}")
    set(arg "abc")
    message("# After change the value of arg.")
    message("arg = ${arg}")
endfunction()
message("=== Call function ===")

Foo(${var})

message("arg = ${arg}")

# 在function中修改外部作用域的值:
# 函数调用处用变量的名字var，而不是它的值${var}
# 在函数内部，set的时候，要加上作用域PARENT_SCOPE

set(var "abc")

function(f1 arg)
    set(${arg} "ABC" PARENT_SCOPE)  # ${arg} == var, 于是相当于set(var "ABC" PARENT_SCOPE)
endfunction()

message("before calling f1, var = ${var}")
f1(var)
message("after calling f1, var = ${var}")


set(var "abc")

function(f1 arg)
    set(${arg} "ABC" PARENT_SCOPE)  # ${arg} == var, 于是相当于set(var "ABC" PARENT_SCOPE)
endfunction()

message("before calling f1, abc = ${abc}")
f1(${var})
message("after calling f1, abc = ${abc}")

# 函数里几个有用的隐含参数：

# ARGC	函数实参的个数
# ARGV	所有实参列表
# ARGN	所有额外实参列表, 即ARGV去掉函数声明时显示指定的实参
# ARGV0	函数第1个实参
# ARGV1	函数第2个实参
# ARGV2	函数第3个实参
# 依次类推	依次类推

function(print_list arg)
    message("ARGC is ${ARGC}")
    message("ARGV is ${ARGV}")
    message("ARGN is ${ARGN}")
    message("ARGV0 is ${ARGV0}")
    message("print_list:")
    foreach(v ${arg})
        message(${v})
    endforeach()
    message("print_list end.")
endfunction()

set(arg hello world maxu)

message("------------ calling with qutoes -----------")     # 使用引号来调用
print_list("${arg}")

message("------------ calling without qutoes -----------")  # 不使用引号调用
print_list(${arg})

# 匹配到*.cpp的所有文件以RELATIVE path为相对路径，得到文件路径
file(GLOB_RECURSE cpp_list RELATIVE /home/mx/develop/sources/StudyNote *.cpp)

print_list("${cpp_list}" a b c d)

# 递归包含头文件
function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})               # 当前路径是一个目录吗，是的话就加入到包含目录
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub}) # 对子目录递归调用，包含
        endif()
    endforeach()
endfunction()

include_sub_directories_recursively("..")

set(SRC)  
list(APPEND SRC a.cpp b.cpp)  
list(APPEND SRC c.cpp d.cpp)  

# 注意此处不能是function(MY_INSTALL arg)
function(MY_INSTALL)
    message("arg is ${arg}")
    message("ARGN is ${ARGN}")
    set(options OPTIONAL FAST)
    set(oneValueArgs DESTINATION RENAME)
    set(multiValueArgs TARGETS CONFIGURATIONS)
    cmake_parse_arguments(INSTALL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    message("ARGC is ${ARGC}")
    message("ARGV is ${ARGV}")
    message("ARGV0 is ${ARGV0}")
    message("INSTALL_OPTIONAL is ${INSTALL_OPTIONAL}")
    message("INSTALL_FAST is ${INSTALL_FAST}")
    message("INSTALL_DESTINATION is ${INSTALL_DESTINATION}")
    message("INSTALL_RENAME is ${INSTALL_RENAME}")
    message("INSTALL_TARGETS is ${INSTALL_TARGETS}")
    message("INSTALL_CONFIGURATIONS is ${INSTALL_CONFIGURATIONS}")
    message("INSTALL_UNPARSED_ARGUMENTS is ${INSTALL_UNPARSED_ARGUMENTS}")
endfunction()

my_install(TARGETS foo bar DESTINATION bin OPTIONAL blub)

LIST(APPEND speech_deal_CHECK_LIBRARY_DIRS /home/peak/programs/robot/lib)

FIND_LIBRARY(speech_deal_LIBRARY name speech_deal PATHS ${speech_deal_CHECK_LIBRARY_DIRS})
message("speech_deal:'${speech_deal_LIBRARY}'")


LIST(APPEND GLOG_CHECK_LIBRARY_DIRS /opt)

find_library(GLOG_LIBRARY NAMES glog PATHS .)
message("GLOG_LIBRARY:'${GLOG_LIBRARY}'")

# copies the file in configuration step and only in this step.
# configure_file(<input> <output> COPYONLY)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/input.txt
    COMMAND echo "Generating input.txt file..."
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/source/input.txt
            ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

message("CMAKE_SOURCE_DIR: '${CMAKE_SOURCE_DIR}'")
message("CMAKE_CURRENT_BINARY_DIR: '${CMAKE_CURRENT_BINARY_DIR}'")


set(TEST_FILE "log.txt")
add_custom_command(
    OUTPUT  ${TEST_FILE}
    COMMAND echo "Generating log.txt file..."
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_FILE} ${TEST_FILE}
    COMMENT  "This is a test"
)
add_custom_target(Test1 ALL DEPENDS ${TEST_FILE} ${CMAKE_CURRENT_BINARY_DIR}/input.txt)

add_custom_command(TARGET Test1
    PRE_BUILD 
    COMMAND echo "executing a fake command"
    COMMENT "This command will be executed before building target Test1"
)

string(REGEX MATCH "^.*(/build)" WORKSPACE_DIR ${CMAKE_BINARY_DIR})
message("WORKSPACE_DIR:'${WORKSPACE_DIR}'")
message("CMAKE_BINARY_DIR:'${CMAKE_BINARY_DIR}'")

# file(INSTALL ...)


# string(REPLACE <match_string>
#        <replace_string> <output variable>
#        <input> [<input>...])

string(REPLACE "abc" "def" hello "aabcdefaa")
message("hello '${hello}'")
