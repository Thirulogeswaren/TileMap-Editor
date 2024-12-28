
target_sources(Editor
    PRIVATE
    map-handler-impl.cpp
    tileset-loader-impl.cpp
    PRIVATE FILE_SET HEADERS
    FILES
    map-handler.h
    tileset-loader.h
    common-types.h
)