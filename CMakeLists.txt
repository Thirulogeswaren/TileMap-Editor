cmake_minimum_required(VERSION 3.16...3.40)

project(Editor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
	SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_Declare(
	spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(spdlog)

# download clay.h - High performance UI layout library
file( 
    DOWNLOAD https://github.com/nicbarker/clay/releases/download/v0.12/clay.h 
    ${CMAKE_SOURCE_DIR}/vendor/clay.h
)

# editor sources
add_subdirectory(source/editor)
