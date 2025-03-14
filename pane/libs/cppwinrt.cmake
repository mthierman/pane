include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}_cppwinrt
    DOWNLOAD_NO_PROGRESS ON
    URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.240405.15"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(${PROJECT_NAME}_cppwinrt)

execute_process(
    COMMAND
        "${${PROJECT_NAME}_cppwinrt_SOURCE_DIR}/bin/cppwinrt" -input sdk -output
        "${${PROJECT_NAME}_cppwinrt_BINARY_DIR}/include"
    )

add_library(
    ${PROJECT_NAME}_cppwinrt
    INTERFACE
    )

add_library(
    ${PROJECT_NAME}::cppwinrt
    ALIAS
    ${PROJECT_NAME}_cppwinrt
    )

target_include_directories(
    ${PROJECT_NAME}_cppwinrt
    SYSTEM
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_cppwinrt_BINARY_DIR}/include>
              $<INSTALL_INTERFACE:include>
    )
