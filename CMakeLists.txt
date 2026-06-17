cmake_minimum_required(VERSION 3.14)

project(name C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE cc "src/cc/*.c")
file(GLOB_RECURSE backdoor "src/backdoor/*.c")
file(GLOB_RECURSE common "src/common/*.c")

add_executable(cc ${cc} ${common})
add_executable(backdoor ${backdoor} ${common})

target_compile_options(backdoor PRIVATE "-static" "-g")
target_link_options(backdoor PRIVATE "--static")
target_compile_options(cc PRIVATE "-g")

target_include_directories(cc PRIVATE "include")
target_include_directories(backdoor PRIVATE "include")
