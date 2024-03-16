# /CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(CLRS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

# adds include directoy to include list
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Adds /common branch as an include path for /tests for shared testing fixtures
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests/common)

add_subdirectory(src)
add_subdirectory(tests)