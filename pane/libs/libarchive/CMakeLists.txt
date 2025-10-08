# FetchContent_Declare(
#     libarchive
#     DOWNLOAD_NO_PROGRESS ON
#     URL
#         https://github.com/libarchive/libarchive/releases/download/v3.8.1/libarchive-3.8.1.tar.xz
#         SOURCE_SUBDIR
#         NULL
# )

# FetchContent_MakeAvailable(libarchive)

# add_library(libarchive INTERFACE)

# add_library(libarchive::libarchive ALIAS libarchive)

# target_include_directories(libarchive SYSTEM INTERFACE ${libarchive_SOURCE_DIR}/include)
