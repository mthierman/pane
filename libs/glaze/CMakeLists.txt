include(FetchContent)

FetchContent_Declare(
    glaze_glaze
    GIT_REPOSITORY "https://github.com/stephenberry/glaze.git"
    GIT_TAG "v5.0.0"
    SOURCE_SUBDIR
    "NULL"
    )

FetchContent_MakeAvailable(glaze_glaze)

add_library(
    glaze_glaze
    INTERFACE
    )

add_library(
    glaze::glaze
    ALIAS
    glaze_glaze
    )

target_include_directories(
    glaze_glaze
    SYSTEM
    INTERFACE "${glaze_glaze_SOURCE_DIR}/include"
    )
