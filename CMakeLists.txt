cmake_minimum_required(VERSION 2.8.11)
# CMP0054: Only interpret if() arguments as variables or keywords when unquoted.
cmake_policy(SET CMP0054 NEW)

project(StaticMode)

enable_testing()

set(CMAKE_CXX_FLAGS "")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Weverything -Wno-c++98-compat -Wno-unused-const-variable")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -Wextra -pedantic")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  # /EHsc enable exceptions (needed by Catch unit testing framework, not by StaticMode)
  # /Za disables language extensions
  # /W4 maximum warning level
  # /WX treat warnings as errors
  # /analyze enable code analysis
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /std:c++14 /EHsc /Za /W4 /analyze")

endif()

add_subdirectory(test)
add_subdirectory(examples)
