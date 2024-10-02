cmake_minimum_required(VERSION 3.10)
project(json_eval)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Clang-tidy. Only run it when needed. Otherwise, it will spit at the google tests included in the project.
#find_program(CLANG_TIDY_EXE NAMES clang-tidy)
#if (CLANG_TIDY_EXE)
#    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
#    # Use -warnings-as-errors=* to make all warnings into errors
#    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=performance-*,cppcoreguidelines-*,clang-analyzer-*,readability-*")
#else ()
#    message(STATUS "clang-tidy not found.")
#endif ()

# Main executable
add_executable(json_eval
        main.cpp
        json_parser.cpp
        expr.cpp
        expr_parser.cpp
        expr_evaluator.cpp
)

target_include_directories(json_eval PRIVATE .)
include_directories(${PROJECT_SOURCE_DIR})

# Tests
option(BUILD_TESTS "Build the tests" OFF)

if (BUILD_TESTS)
    add_subdirectory(external/gtest)
    include_directories(external/gtest/googletest/include)
    set(TEST_SOURCES
            tests/test_main.cpp
            tests/test_json_parser.cpp
            tests/test_expr_parser.cpp
            tests/test_expr_evaluator.cpp
    )
    add_executable(tests ${TEST_SOURCES} json_parser.cpp expr.cpp expr_parser.cpp expr_evaluator.cpp)
    target_link_libraries(tests PRIVATE gtest gtest_main pthread)
    add_test(NAME AllTests COMMAND tests)
    add_custom_target(run-tests
            COMMAND tests
            DEPENDS tests
            COMMENT "Running all tests"
    )
endif ()