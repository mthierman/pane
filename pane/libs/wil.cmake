include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}_wil
    DOWNLOAD_NO_PROGRESS ON
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.240803.1"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(${PROJECT_NAME}_wil)

add_library(
    ${PROJECT_NAME}_wil
    INTERFACE
    )

add_library(
    ${PROJECT_NAME}::wil
    ALIAS
    ${PROJECT_NAME}_wil
    )

target_include_directories(
    ${PROJECT_NAME}_wil
    SYSTEM
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_wil_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
    )
