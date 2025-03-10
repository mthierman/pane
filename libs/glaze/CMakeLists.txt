include(FetchContent)

FetchContent_Declare(
    glaze
    GIT_REPOSITORY "https://github.com/stephenberry/glaze.git"
    GIT_TAG "v5.0.0"
    SOURCE_SUBDIR
    "NULL"
    )

FetchContent_MakeAvailable(glaze)

add_library(
    glaze
    INTERFACE
    )

add_library(
    glaze::glaze
    ALIAS
    glaze
    )

target_include_directories(
    glaze
    SYSTEM
    INTERFACE "${glaze_SOURCE_DIR}/include"
    )
