include(FetchContent)

FetchContent_Declare(
    ada_ada
    URL "https://github.com/ada-url/ada/releases/download/v3.1.3/singleheader.zip"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(ada_ada)

add_library(ada_ada)

add_library(
    ada::ada
    ALIAS
    ada_ada
    )

target_sources(ada_ada PRIVATE "${ada_ada_SOURCE_DIR}/ada.cpp")

target_include_directories(
    ada_ada
    SYSTEM
    INTERFACE "${ada_ada_SOURCE_DIR}/include"
    )

target_compile_features(
    ada_ada
    PRIVATE c_std_17
            cxx_std_23
    )

target_compile_options(
    ada_ada
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
    ada_ada
    PRIVATE
    /WX
    )
