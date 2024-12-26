
add_executable(${CMAKE_PROJECT_NAME}
    editor.h
    editor.cpp
)

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC SFML::Graphics spdlog::spdlog_header_only)
