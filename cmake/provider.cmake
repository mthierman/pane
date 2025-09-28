cmake_minimum_required(VERSION 3.31)

include(FetchContent)

FetchContent_Declare(
    ada
    DOWNLOAD_NO_PROGRESS ON
    URL https://github.com/ada-url/ada/releases/download/v3.3.0/singleheader.zip SOURCE_SUBDIR NULL
)

macro(pane_provide_dependency method package_name)
    if("${package_name}" STREQUAL "ada")
        FetchContent_MakeAvailable(${package_name})
        set(${package_name}_FOUND TRUE)

        if(NOT TARGET ada::ada)
            add_library(ada)

            add_library(ada::ada-url ALIAS ada)

            target_sources(ada PRIVATE ${ada_SOURCE_DIR}/ada.cpp)

            target_include_directories(ada SYSTEM INTERFACE ${ada_SOURCE_DIR})

            target_compile_features(ada PRIVATE c_std_17 cxx_std_23)
        endif()
    endif()
endmacro()

cmake_language(SET_DEPENDENCY_PROVIDER pane_provide_dependency SUPPORTED_METHODS FIND_PACKAGE)
