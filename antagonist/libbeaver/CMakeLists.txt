cmake_minimum_required(VERSION 3.21.2)
project(libbeaver)
find_package(OpenCV REQUIRED)
add_library(beaver SHARED 
    src/beaver.cc
    src/beaver_impl.cc
)
target_include_directories(beaver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/externals
)

target_compile_options(beaver PRIVATE -Wall -Wextra -Werror -pedantic -Wold-style-cast)
set_target_properties(beaver PROPERTIES CXX_STANDARD 20)

target_link_libraries(beaver PRIVATE ${OpenCV_LIBS})