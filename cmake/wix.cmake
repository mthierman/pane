find_program(WIX_EXECUTABLE wix)

if(
    NOT WIX_EXECUTABLE
    OR "{WIX_EXECUTABLE}" MATCHES "NOTFOUND"
    OR NOT EXISTS "${WIX_EXECUTABLE}"
)
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
