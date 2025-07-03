cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_FLAGS "-Wall -Werror -std=c17")
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(leocc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(cmocka REQUIRED)
include_directories(${CMOCKA_INCLUDE_DIR})
# leocc
add_executable(leocc src/leocc.c)
# Tests
add_executable(tests tests/main.c)
target_link_libraries(tests cmocka)
