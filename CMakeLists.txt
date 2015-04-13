project(DSA_HW2)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set(CMAKE_BUILD_TYPE DEBUG)
add_executable(${PROJECT_NAME} ${SRC_LIST})

