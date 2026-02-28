cmake_minimum_required(VERSION 3.21.2)
project(libzork)

add_subdirectory(libzork)

add_executable(zorkxplorer
    src/main.cc
    src/options.cc
)

target_include_directories(zorkxplorer PRIVATE
    src/
    libzork/include/
)

target_compile_options(zorkxplorer PRIVATE
    -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast
)

target_link_libraries(zorkxplorer PRIVATE zork)