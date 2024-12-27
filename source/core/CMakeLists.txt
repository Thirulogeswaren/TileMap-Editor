
target_sources(Editor
    PRIVATE
    ts_loader_impl.cpp
    umap_handler_impl.cpp
    PRIVATE FILE_SET HEADERS
    FILES
    common.h
    ts_loader.h
    umap_handler.h
)