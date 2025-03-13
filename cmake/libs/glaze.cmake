include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}_glaze
    DOWNLOAD_NO_PROGRESS ON
    URL "https://github.com/stephenberry/glaze/archive/refs/tags/v5.0.0.zip"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(${PROJECT_NAME}_glaze)

add_library(
    ${PROJECT_NAME}_glaze
    INTERFACE
    )

add_library(
    ${PROJECT_NAME}::glaze
    ALIAS
    ${PROJECT_NAME}_glaze
    )

target_include_directories(
    ${PROJECT_NAME}_glaze
    SYSTEM
    INTERFACE "${${PROJECT_NAME}_glaze_SOURCE_DIR}/include"
    )
