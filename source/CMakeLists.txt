
add_executable(${CMAKE_PROJECT_NAME} editor/editor.cpp)

add_subdirectory(core)
add_subdirectory(editor)

add_subdirectory(util)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE FILE_SET HEADERS BASE_DIRS 
    core
    editor
    util
)

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/source)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML SFML::Graphics spdlog::spdlog_header_only)
