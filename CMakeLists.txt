cmake_minimum_required(VERSION 3.20)

project(Editor LANGUAGES CXX)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Ob0 /Od /RTC1")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

execute_process(
	COMMAND ${CMAKE_COMMAND} -E make_directory bin
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# ImGui backend includes SFML and ImGui
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(editor_src editor.cc
	include/core/ts_loader_impl.cc
	include/core/umap_handler_impl.cc
	
	include/util/console.cc
	
	include/menubar.cc
	include/tileset.cc
	include/viewport.cc
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E make_directory bin/${CMAKE_BUILD_TYPE}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy assets/cousine.ttf bin/${CMAKE_BUILD_TYPE}
		COMMAND ${CMAKE_COMMAND} -E copy assets/imgui.ini bin/${CMAKE_BUILD_TYPE}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	
	add_executable(${PROJECT_NAME} ${editor_src})

else(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR "Darwin")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy assets/cousine.ttf bin
		COMMAND ${CMAKE_COMMAND} -E copy assets/imgui.ini bin
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	add_executable(${PROJECT_NAME} ${editor_src})
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML unofficial::nativefiledialog::nfd)
