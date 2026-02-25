cmake_minimum_required(VERSION 3.21.2)
project(antagonist)
add_subdirectory(libbeaver)
add_executable(beaver_run
    src/main.cc
)
target_compile_options(beaver_run PRIVATE -Wall -Wextra -Werror -pedantic -Wold-style-cast)
target_compile_features(beaver_run PRIVATE cxx_std_20)
set_target_properties(beaver_run PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(beaver_run PRIVATE beaver)