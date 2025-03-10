include(FetchContent)

FetchContent_Declare(
    wil
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.240803.1"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(wil)

add_library(
    wil
    INTERFACE
    )

add_library(
    microsoft::wil
    ALIAS
    wil
    )

target_include_directories(
    wil
    SYSTEM
    INTERFACE "${wil_SOURCE_DIR}/include"
    )
