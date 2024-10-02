cmake_minimum_required(VERSION 3.10)
project(json_eval)

set(CMAKE_CXX_STANDARD 17)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if (CLANG_TIDY_EXE)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")
    # Use -warnings-as-errors=* to make all warnings into errors
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-header-filter=include;-checks=performance-*,cppcoreguidelines-*,clang-analyzer-*,readability-*")
else ()
    message(STATUS "clang-tidy not found.")
endif ()

add_executable(json_eval
        main.cpp
        json_parser.cpp
        expr.cpp
        expr_parser.cpp
        expr_evaluator.cpp
)

target_include_directories(json_eval PRIVATE .)

