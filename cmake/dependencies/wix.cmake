find_program(WIX_EXECUTABLE wix NO_CACHE)

if(NOT IS_EXECUTABLE "${WIX_EXECUTABLE}")
    message(STATUS "Wix not found")
else()
    message(STATUS "Wix found: ${WIX_EXECUTABLE}")

    if(NOT TARGET wix)
        add_executable(wix IMPORTED GLOBAL)
        set_target_properties(
            wix
            PROPERTIES IMPORTED_LOCATION "${WIX_EXECUTABLE}"
        )
    endif()
endif()
