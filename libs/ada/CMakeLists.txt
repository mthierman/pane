include(FetchContent)

FetchContent_Declare(
    pane_ada
    URL "https://github.com/ada-url/ada/releases/download/v3.2.1/singleheader.zip"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(pane_ada)

add_library(pane_ada)

add_library(
    pane::ada
    ALIAS
    pane_ada
    )

target_sources(pane_ada PRIVATE "${pane_ada_SOURCE_DIR}/ada.cpp")

target_include_directories(
    pane_ada
    SYSTEM
    INTERFACE "${pane_ada_SOURCE_DIR}/include"
    )

target_compile_features(
    pane_ada
    PRIVATE c_std_17
            cxx_std_23
    )

target_compile_options(
    pane_ada
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
    pane_ada
    PRIVATE
    /WX
    )
