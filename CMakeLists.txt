cmake_minimum_required(VERSION 3.26)

project(Editor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# ImGui backend includes SFML and ImGui
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)

set(SRC_FILES editor.cc util/console.cc
	core/settings.cc
	core/hbuffer.cc
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(_WIN32_ENTRY__CC)
	add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})
else()
	add_executable(${PROJECT_NAME} ${SRC_FILES})
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML unofficial::nativefiledialog::nfd)
