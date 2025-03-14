include(FetchContent)

FetchContent_Declare(
    ${PROJECT_NAME}_ada
    DOWNLOAD_NO_PROGRESS ON
    URL "https://github.com/ada-url/ada/releases/download/v3.2.1/singleheader.zip"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(${PROJECT_NAME}_ada)

add_library(${PROJECT_NAME}_ada)

add_library(
    ${PROJECT_NAME}::ada
    ALIAS
    ${PROJECT_NAME}_ada
    )

target_sources(${PROJECT_NAME}_ada PRIVATE "${${PROJECT_NAME}_ada_SOURCE_DIR}/ada.cpp")

target_include_directories(
    ${PROJECT_NAME}_ada
    SYSTEM
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_ada_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>
    )

target_compile_features(
    ${PROJECT_NAME}_ada
    PRIVATE c_std_17
            cxx_std_23
    )
