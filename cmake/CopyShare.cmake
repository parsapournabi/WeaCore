# Copying automatically share directory into the build path.

function(copy_share TARGET_NAME)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/share")
        add_custom_command(
            TARGET ${TARGET_NAME}

            POST_BUILD

            COMMAND ${CMAKE_COMMAND}
                -E copy_directory

                "${CMAKE_CURRENT_SOURCE_DIR}/share"


                "$<TARGET_FILE_DIR:${TARGET_NAME}>/share"
            )
    endif()

endfunction()

