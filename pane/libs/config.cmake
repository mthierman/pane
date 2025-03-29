add_library(
    pane_config
    INTERFACE
    )

add_library(
    pane::config
    ALIAS
    pane_config
    )

target_compile_features(
    pane_config
    INTERFACE c_std_17
              cxx_std_23
    )

target_compile_definitions(
    pane_config
    INTERFACE UNICODE
              WIN32_LEAN_AND_MEAN
              NOMINMAX
              GDIPVER=0x0110
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options
target_compile_options(
    pane_config
    INTERFACE $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
              /W4
              /WX
              /MP
              /utf-8
              /bigobj
              /diagnostics:color
              /permissive-
              /Zc:__cplusplus,__STDC__,enumTypes,templateScope,throwingNew,preprocessor
              >
              $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
              # -Wall -Werror
              >
    )

# https://learn.microsoft.com/en-us/cpp/build/reference/linker-options
target_link_options(
    pane_config
    INTERFACE
    $<$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>:
    /WX
    >
    $<$<CXX_COMPILER_FRONTEND_VARIANT:GNU>:
    # -Wl,/WX
    >
    )
