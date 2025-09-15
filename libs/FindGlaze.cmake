include(FetchContent)

FetchContent_Declare(
    glaze
    DOWNLOAD_NO_PROGRESS ON
    URL "https://github.com/stephenberry/glaze/archive/refs/tags/v5.6.0.zip" SOURCE_SUBDIR "NULL"
)

FetchContent_MakeAvailable(glaze)

add_library(glaze INTERFACE)

target_include_directories(glaze SYSTEM INTERFACE "${glaze_SOURCE_DIR}/include")
