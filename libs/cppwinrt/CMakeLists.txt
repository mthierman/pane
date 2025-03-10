include(FetchContent)

FetchContent_Declare(
    cppwinrt
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.240405.15"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(cppwinrt)

execute_process(
    COMMAND
        "${cppwinrt_SOURCE_DIR}/bin/cppwinrt" -input sdk -output
        "${CMAKE_CURRENT_BINARY_DIR}/include"
    )

add_library(
    cppwinrt
    INTERFACE
    )

add_library(
    microsoft::cppwinrt
    ALIAS
    cppwinrt
    )

target_include_directories(
    cppwinrt
    SYSTEM
    INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
