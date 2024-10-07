project(cpp-hse)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

include(cmake/BuildFlags.cmake)
include(cmake/TestSolution.cmake)

find_package(Catch REQUIRED)

include_directories(util)

# 0
add_subdirectory(tasks/multiplication)


# 1
add_subdirectory(tasks/next_even)
add_subdirectory(tasks/is_leap)
add_subdirectory(tasks/chocolate)
add_subdirectory(tasks/min_divisor)
add_subdirectory(tasks/fibonacci)
add_subdirectory(tasks/reduce_fraction)
add_subdirectory(tasks/palindrome)
add_subdirectory(tasks/password)


# 2
add_subdirectory(tasks/sort_students)
add_subdirectory(tasks/tests_checking)
add_subdirectory(tasks/admission)
add_subdirectory(tasks/scorer)
add_subdirectory(tasks/unixpath)


# 3
add_subdirectory(tasks/provinces)
add_subdirectory(tasks/search)
add_subdirectory(tasks/rational)
add_subdirectory(tasks/stack)
add_subdirectory(tasks/queue)

# project
add_subdirectory(tasks/archiver)
