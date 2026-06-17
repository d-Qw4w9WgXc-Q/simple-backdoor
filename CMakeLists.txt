cmake_minimum_required(VERSION 3.14)

project(name C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE c2 "src/c2/*.c")
file(GLOB_RECURSE backdoor "src/backdoor/*.c")
file(GLOB_RECURSE common "src/common/*.c")

add_executable(c2 ${c2} ${common})
add_executable(backdoor ${backdoor} ${common})

target_compile_options(backdoor PRIVATE "-static" "-g")
target_link_options(backdoor PRIVATE "--static")
target_compile_options(c2 PRIVATE "-g")

target_include_directories(c2 PRIVATE "include")
target_include_directories(backdoor PRIVATE "include")
