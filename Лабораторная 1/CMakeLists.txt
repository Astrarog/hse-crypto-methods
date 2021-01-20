cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(Hill LANGUAGES CXX)

add_executable(${PROJECT_NAME} "main.cpp")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<PLATFORM_ID:Windows>: "WINDOWS_PLATFORM">)
