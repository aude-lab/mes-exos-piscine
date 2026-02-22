cmake_minimum_required(VERSION 3.21.2)
project(libzork LANGUAGES CXX)

add_library(libzork SHARED
"src/runner/choice_impl.cc"
"src/runner/smart_impl.cc"
"src/store/store.cc"
"src/runner/smart.cc"
"src/store/store_impl.cc"
"src/runner/runner.cc"
"src/story/node.cc"
"src/runner/html_impl.cc"
"src/story/node_impl.cc"
"src/story/story.cc"
"src/runner/html.cc"
"src/vars/condition_impl.cc"
"src/exceptions.cc"
"src/runner/choice.cc"
"src/vars/condition.cc"
"src/story/story_impl.cc"
"src/vars/action.cc"
"src/vars/action_impl.cc"
"src/runner/interactive.cc"
)
target_compile_options(libzork PUBLIC -Wall -Wextra -Werror -pedantic -std=c++20 -Wold-style-cast -g)

set_target_properties(libzork PROPERTIES  OUTPUT_NAME "zork")

target_include_directories(libzork PUBLIC "include/")
target_include_directories(libzork PUBLIC "src/.")

add_executable(exec_test "zorkxplorer/src/main.cc" "zorkxplorer/src/options.cc")

target_include_directories(exec_test PUBLIC "include/")
target_include_directories(exec_test PUBLIC "zorkxplorer/src/")
target_link_libraries(exec_test libzork)

find_library(yaml-cpp NAME yaml-cpp)
target_link_libraries(libzork PUBLIC yaml-cpp)
