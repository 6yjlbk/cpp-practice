cmake_minimum_required(VERSION 3.20)

project(FEMSolver VERSION 0.1.0 LANGUAGES CXX)

add_executable(fem_solver main.cpp)

target_compile_features(fem_solver PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(fem_solver PRIVATE /W4 /permissive-)
else()
    target_compile_options(fem_solver PRIVATE -Wall -Wextra -Wpedantic)
endif()
