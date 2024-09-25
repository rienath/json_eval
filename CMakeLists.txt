cmake_minimum_required(VERSION 3.10)
project(json_eval)

set(CMAKE_CXX_STANDARD 17)

add_executable(json_eval
        main.cpp
        json_parser.cpp
        expr.cpp
        expr_parser.cpp
        expr_evaluator.cpp
)

target_include_directories(json_eval PRIVATE .)