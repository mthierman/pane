include(FetchContent)

FetchContent_Declare(
    sqlite
    DOWNLOAD_NO_PROGRESS ON
    URL
        "https://sqlite.org/2025/sqlite-amalgamation-3500400.zip"
        SOURCE_SUBDIR
        "NULL"
)

FetchContent_MakeAvailable(sqlite)

add_library(sqlite)

target_sources(sqlite PRIVATE "${sqlite_SOURCE_DIR}/sqlite3.c")

target_include_directories(ada SYSTEM INTERFACE "${sqlite_SOURCE_DIR}")

target_compile_features(ada PRIVATE c_std_17 cxx_std_23)
