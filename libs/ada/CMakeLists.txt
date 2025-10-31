FetchContent_Declare(
    ada
    DOWNLOAD_NO_PROGRESS ON
    URL
        https://github.com/ada-url/ada/releases/download/v${PANE_ADA_VERSION}/singleheader.zip
        SOURCE_SUBDIR
        NULL
)

FetchContent_MakeAvailable(ada)

add_library(libs_ada)

add_library(ada::ada-url ALIAS libs_ada)

target_sources(libs_ada PRIVATE ${ada_SOURCE_DIR}/ada.cpp)

target_include_directories(libs_ada SYSTEM PUBLIC ${ada_SOURCE_DIR})

target_compile_features(libs_ada PRIVATE c_std_17 cxx_std_23)
