add_executable(beaver_run "main.cc")

set_target_properties(beaver_run PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)


target_include_directories(beaver_run PRIVATE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(beaver_run PRIVATE libbeaver)

add_dependencies(beaver_run libbeaver)

add_custom_command(TARGET beaver_run POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/test/video
        $<TARGET_FILE_DIR:beaver_run>/video
)

target_compile_options(beaver_run PUBLIC -Wall -Wextra -Werror -pedantic -Wold-style-cast)
