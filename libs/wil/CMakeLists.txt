include(FetchContent)

FetchContent_Declare(
    pane_wil
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.240803.1"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(pane_wil)

add_library(
    pane_wil
    INTERFACE
    )

add_library(
    pane::wil
    ALIAS
    pane_wil
    )

target_include_directories(
    pane_wil
    SYSTEM
    INTERFACE "${pane_wil_SOURCE_DIR}/include"
    )
