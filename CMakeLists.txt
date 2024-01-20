cmake_minimum_required(VERSION 3.26)

project(Editor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ImGui backend includes SFML and ImGui
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable(${PROJECT_NAME} editor.cc
    util/console.cc
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
