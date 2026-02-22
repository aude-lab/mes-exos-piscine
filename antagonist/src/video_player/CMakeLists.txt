add_library(video_player STATIC "video_player.cc")

find_package(OpenCV REQUIRED)

set_target_properties(video_player PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_include_directories(video_player PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(video_player PRIVATE opencv_core opencv_highgui opencv_imgproc)

target_compile_options(video_player PUBLIC -Wall -Wextra -Werror -pedantic -Wold-style-cast)