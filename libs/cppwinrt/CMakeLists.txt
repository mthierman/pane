include(FetchContent)

FetchContent_Declare(
    pane_cppwinrt
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.240405.15"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(pane_cppwinrt)

execute_process(
    COMMAND
        "${pane_cppwinrt_SOURCE_DIR}/bin/cppwinrt" -input sdk -output
        "${pane_cppwinrt_BINARY_DIR}/include"
    )

add_library(
    pane_cppwinrt
    INTERFACE
    )

add_library(
    pane::cppwinrt
    ALIAS
    pane_cppwinrt
    )

target_include_directories(
    pane_cppwinrt
    SYSTEM
    INTERFACE "${pane_cppwinrt_BINARY_DIR}/include"
    )
