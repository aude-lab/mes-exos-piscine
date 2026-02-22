add_library(libbeaver SHARED "beaver.cc" "beaver_impl.cc")

find_package(OpenCV REQUIRED)

set_target_properties(libbeaver PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        OUTPUT_NAME "beaver"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(libbeaver PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(libbeaver PRIVATE opencv_core opencv_highgui opencv_imgproc video_player)

target_compile_options(libbeaver PUBLIC  -Wall -Wextra -Werror -pedantic -Wold-style-cast)
