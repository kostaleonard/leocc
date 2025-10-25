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
add_library(list src/list.c)
target_link_libraries(list cexception)
add_library(token src/token.c)
target_link_libraries(token cexception)
add_library(scanner src/scanner.c)
target_link_libraries(scanner list token cexception)
add_library(parser src/parser.c)
target_link_libraries(parser cexception)
add_executable(leocc src/leocc.c)
target_link_libraries(leocc scanner)
# Tests
add_executable(tests tests/main.c)
add_library(test_list tests/src/test_list.c)
target_link_libraries(test_list list cexception)
add_library(test_token tests/src/test_token.c)
target_link_libraries(test_token token cexception)
add_library(test_scanner tests/src/test_scanner.c)
target_link_libraries(test_scanner list token scanner cexception)
add_library(test_parser tests/src/test_parser.c)
target_link_libraries(test_parser parser cexception)
target_link_libraries(tests test_list test_token test_scanner test_parser cmocka)
