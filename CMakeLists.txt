cmake_minimum_required(VERSION 3.24...3.40)

project(Editor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/bin)

include(cmake/copy-assets.cmake)
include(cmake/dependencies.cmake)

# editor sources
add_subdirectory(source)
