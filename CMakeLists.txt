cmake_minimum_required(VERSION 3.26)

project(Editor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

execute_process(
	COMMAND ${CMAKE_COMMAND} -E make_directory bin
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ImGui backend includes SFML and ImGui
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)

set(editor_src editor.cc
	core/tileset.cc
	core/hbuffer.cc
	
	util/console.cc
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E make_directory bin/${CMAKE_BUILD_TYPE}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy util/assets/cousine.ttf bin/${CMAKE_BUILD_TYPE}
		COMMAND ${CMAKE_COMMAND} -E copy util/assets/imgui.ini bin/${CMAKE_BUILD_TYPE}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	
	add_compile_definitions(_WIN32_ENTRY__CC)
	add_executable(${PROJECT_NAME} WIN32 ${editor_src})

else(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR "Darwin")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy util/assets/cousine.ttf bin
		COMMAND ${CMAKE_COMMAND} -E copy util/assets/imgui.ini bin
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	add_executable(${PROJECT_NAME} ${editor_src})
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML unofficial::nativefiledialog::nfd)
