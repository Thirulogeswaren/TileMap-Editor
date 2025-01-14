
include(FetchContent)

FetchContent_Declare(
    SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)
    
FetchContent_Declare( 
    spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
    
FetchContent_MakeAvailable(spdlog)
    
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG docking
)

FetchContent_MakeAvailable(imgui)
    
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
# set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG master
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(ImGui-SFML)
