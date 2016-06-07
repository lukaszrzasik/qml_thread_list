cmake_minimum_required (VERSION 2.8)

project(threadListQml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(compiler_options)

add_subdirectory(src)
