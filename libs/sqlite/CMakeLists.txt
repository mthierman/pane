FetchContent_Declare(
    sqlite
    DOWNLOAD_NO_PROGRESS ON
    URL https://sqlite.org/2026/sqlite-amalgamation-${PANE_SQLITE_VERSION}.zip SOURCE_SUBDIR NULL
)

FetchContent_MakeAvailable(sqlite)

add_library(libs_sqlite)

add_library(sqlite::sqlite ALIAS libs_sqlite)

target_sources(libs_sqlite PRIVATE ${sqlite_SOURCE_DIR}/sqlite3.c)

target_include_directories(libs_sqlite SYSTEM PUBLIC ${sqlite_SOURCE_DIR})

target_compile_features(libs_sqlite PRIVATE c_std_17 cxx_std_23)
