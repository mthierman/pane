include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}_ada
    URL "https://github.com/ada-url/ada/releases/download/v3.2.1/singleheader.zip"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(${PROJECT_NAME}_ada)

add_library(${PROJECT_NAME}_ada)

add_library(
    ${PROJECT_NAME}::ada
    ALIAS
    ${PROJECT_NAME}_ada
    )

target_sources(${PROJECT_NAME}_ada PRIVATE "${${PROJECT_NAME}_ada_SOURCE_DIR}/ada.cpp")

target_include_directories(
    ${PROJECT_NAME}_ada
    SYSTEM
    INTERFACE "${${PROJECT_NAME}_ada_SOURCE_DIR}/include"
    )

target_compile_features(
    ${PROJECT_NAME}_ada
    PRIVATE c_std_17
            cxx_std_23
    )

target_compile_options(
    ${PROJECT_NAME}_ada
    PRIVATE /W0
            /WX
            /MP
            /utf-8
            /bigobj
            /diagnostics:caret
            /permissive-
            /Zc:__cplusplus,__STDC__,enumTypes,templateScope,throwingNew,preprocessor
    )

target_link_options(
    ${PROJECT_NAME}_ada
    PRIVATE
    /WX
    )
