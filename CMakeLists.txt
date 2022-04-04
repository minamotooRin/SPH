cmake_minimum_required(VERSION 3.4.1)
project(SPH)

add_definitions("-Wall -std=c++14 -O3") # 编译选项
# add_definitions("-Wall -std=c++17 -O0 -g") # 编译选项

# 打印信息
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib SRC_LIST)
add_library(SPH SHARED
    ${SRC_LIST}
)

add_executable(main
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(main SPH)