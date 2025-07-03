cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_FLAGS "-Wall -Werror -std=c17 -DCEXCEPTION_USE_CONFIG_FILE")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(leocc)
find_package(cmocka REQUIRED)
include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/include
    CException/lib
    ${CMOCKA_INCLUDE_DIR}
)
# CException
add_library(cexception CException/lib/CException.c)
# leocc
add_executable(leocc src/leocc.c)
add_library(fib src/fib.c)
target_link_libraries(fib cexception)
target_link_libraries(leocc fib)
# Tests
add_executable(tests tests/main.c)
add_library(test_fib tests/src/test_fib.c)
target_link_libraries(test_fib fib)
target_link_libraries(test_fib cexception)
target_link_libraries(tests test_fib)
target_link_libraries(tests cmocka)
