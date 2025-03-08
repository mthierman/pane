include(FetchContent)

FetchContent_Declare(
    microsoft_wil
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.240803.1"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(microsoft_wil)

add_library(
    microsoft_wil
    INTERFACE
    )

add_library(
    microsoft::wil
    ALIAS
    microsoft_wil
    )

target_include_directories(
    microsoft_wil
    SYSTEM
    INTERFACE "${microsoft_wil_SOURCE_DIR}/include"
    )
