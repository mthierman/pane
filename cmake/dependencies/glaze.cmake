FetchContent_Declare(
    glaze
    DOWNLOAD_NO_PROGRESS ON
    URL https://github.com/stephenberry/glaze/archive/refs/tags/v5.7.2.zip SOURCE_SUBDIR NULL
)

FetchContent_MakeAvailable(glaze)

add_library(glaze INTERFACE)

add_library(stephenberry::glaze ALIAS glaze)

target_include_directories(glaze SYSTEM INTERFACE ${glaze_SOURCE_DIR}/include)
