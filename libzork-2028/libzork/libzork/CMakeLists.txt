cmake_minimum_required(VERSION 3.21.2)
find_library(YAML_CPP_LIB yaml-cpp)
add_library(zork SHARED    
src/exceptions.cc
    src/runner/runner.cc
    src/runner/interactive.cc
    src/runner/choice.cc
    src/runner/choice_impl.cc
    src/runner/html.cc
    src/runner/html_impl.cc
    src/runner/smart.cc
    src/runner/smart_impl.cc
    src/store/store.cc
    src/store/store_impl.cc
    src/story/choice.cc
    src/story/node.cc
    src/story/node_impl.cc
    src/story/story.cc
    src/story/story_impl.cc
    src/vars/action.cc
    src/vars/action_impl.cc
    src/vars/condition.cc
    src/vars/condition_impl.cc
)
target_compile_options(zork PUBLIC
    -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast
)
set_target_properties(zork PROPERTIES OUTPUT_NAME "zork")
target_include_directories(zork PUBLIC include/)
target_include_directories(zork PRIVATE src/)
target_link_libraries(zork PUBLIC ${YAML_CPP_LIB})
target_link_options(zork PRIVATE -Wl,--no-undefined)
