add_library(
    config
    INTERFACE
    )

add_library(
    pane::config
    ALIAS
    config
    )

target_compile_features(
    config
    INTERFACE c_std_17
              cxx_std_23
    )

target_compile_definitions(
    config
    INTERFACE UNICODE
              WIN32_LEAN_AND_MEAN
              NOMINMAX
              GDIPVER=0x0110
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options
target_compile_options(
    config
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
    config
    INTERFACE
    /WX
    )
