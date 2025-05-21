add_custom_target(submission)

# Copy submission Files
add_custom_command(
        TARGET submission POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src
                ${CMAKE_SOURCE_DIR}/submission/src)

add_custom_command(
        TARGET submission POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/shaders
                ${CMAKE_SOURCE_DIR}/submission/shaders)

add_custom_command(
        TARGET submission POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/res
                ${CMAKE_SOURCE_DIR}/submission/res)
