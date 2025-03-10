add_library(
    ${PROJECT_NAME}_config
    INTERFACE
    )

add_library(
    pane::config
    ALIAS
    ${PROJECT_NAME}_config
    )

target_compile_features(
    ${PROJECT_NAME}_config
    INTERFACE c_std_17
              cxx_std_23
    )

target_compile_definitions(
    ${PROJECT_NAME}_config
    INTERFACE UNICODE
              WIN32_LEAN_AND_MEAN
              NOMINMAX
              GDIPVER=0x0110
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options
target_compile_options(
    ${PROJECT_NAME}_config
    INTERFACE /W4
              /WX
              /MP
              /utf-8
              /bigobj
              /diagnostics:caret
              /permissive-
              /Zc:__cplusplus,__STDC__,enumTypes,templateScope,throwingNew,preprocessor
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/linker-options
target_link_options(
    ${PROJECT_NAME}_config
    INTERFACE
    /WX
    )
