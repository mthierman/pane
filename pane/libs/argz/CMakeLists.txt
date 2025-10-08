FetchContent_Declare(
    argz
    DOWNLOAD_NO_PROGRESS ON
    URL
        https://github.com/stephenberry/argz/archive/refs/tags/v${ARGZ_VERSION}.zip
        SOURCE_SUBDIR
        NULL
)

FetchContent_MakeAvailable(argz)

add_library(argz INTERFACE)

add_library(stephenberry::argz ALIAS argz)

target_include_directories(argz SYSTEM INTERFACE ${argz_SOURCE_DIR}/include)
