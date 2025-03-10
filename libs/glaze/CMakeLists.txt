include(FetchContent)

FetchContent_Declare(
    pane_glaze
    URL "https://github.com/stephenberry/glaze/archive/refs/tags/v5.0.0.zip"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(pane_glaze)

add_library(
    pane_glaze
    INTERFACE
    )

add_library(
    pane::glaze
    ALIAS
    pane_glaze
    )

target_include_directories(
    pane_glaze
    SYSTEM
    INTERFACE "${pane_glaze_SOURCE_DIR}/include"
    )
