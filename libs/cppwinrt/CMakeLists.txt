include(FetchContent)

FetchContent_Declare(
    microsoft_cppwinrt
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.240405.15"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(microsoft_cppwinrt)

execute_process(
    COMMAND
        "${microsoft_cppwinrt_SOURCE_DIR}/bin/cppwinrt" -input sdk -output
        "${CMAKE_CURRENT_BINARY_DIR}/include"
    )

add_library(
    microsoft_cppwinrt
    INTERFACE
    )

add_library(
    microsoft::cppwinrt
    ALIAS
    microsoft_cppwinrt
    )

target_include_directories(
    microsoft_cppwinrt
    SYSTEM
    INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
