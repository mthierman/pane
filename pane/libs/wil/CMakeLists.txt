FetchContent_Declare(
    wil
    DOWNLOAD_NO_PROGRESS ON
    URL
        https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/${PANE_WIL_VERSION}
        SOURCE_SUBDIR
        NULL
)

FetchContent_MakeAvailable(wil)

add_library(wil INTERFACE)

add_library(microsoft::wil ALIAS wil)

target_include_directories(wil SYSTEM INTERFACE ${wil_SOURCE_DIR}/include)
