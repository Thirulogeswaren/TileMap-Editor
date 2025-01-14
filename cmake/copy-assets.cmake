
# copying assets/ to bin

execute_process(
	COMMAND ${CMAKE_COMMAND} -E make_directory bin
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

execute_process(
	COMMAND ${CMAKE_COMMAND} -E copy assets/cousine.ttf bin
	COMMAND ${CMAKE_COMMAND} -E copy assets/imgui.ini bin
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

message(STATUS "Assets are copied to bin/")
