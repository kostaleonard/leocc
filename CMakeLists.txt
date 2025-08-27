cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_FLAGS "-Wall -Werror -std=c17 -DCEXCEPTION_USE_CONFIG_FILE")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_definitions(_GNU_SOURCE)
project(leocc)
find_package(cmocka REQUIRED)
include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
    # So that CException can see its own header:
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/CException/CException/lib
    # So that CException can see our configuration file:
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/CException/config
    ${CMOCKA_INCLUDE_DIR}
)
# CException
add_library(cexception submodules/CException/CException/lib/CException.c)
# leocc
add_executable(leocc src/leocc.c)
add_library(fib src/fib.c)
target_link_libraries(fib cexception)
target_link_libraries(leocc fib)
add_library(list src/list.c)
target_link_libraries(list cexception)
# Tests
add_executable(tests tests/main.c)
add_library(test_fib tests/src/test_fib.c)
target_link_libraries(test_fib fib cexception)
add_library(test_list tests/src/test_list.c)
target_link_libraries(test_list list cexception)
target_link_libraries(tests test_fib test_list cmocka)
