include(FetchContent)

FetchContent_Declare(
    ada
    DOWNLOAD_NO_PROGRESS ON
    URL
        "https://github.com/ada-url/ada/releases/download/v3.2.7/singleheader.zip"
        SOURCE_SUBDIR
        "NULL"
)

FetchContent_MakeAvailable(ada)

add_library(ada)

target_sources(ada PRIVATE "${ada_SOURCE_DIR}/ada.cpp")

target_include_directories(ada SYSTEM INTERFACE "${ada_SOURCE_DIR}")

target_compile_features(ada PRIVATE c_std_17 cxx_std_23)
