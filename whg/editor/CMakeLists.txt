cmake_minimum_required(VERSION 3.22)
project(AppProject)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

add_executable(editor main.cpp)

target_link_libraries(editor sfml-graphics sfml-window sfml-system sfml-audio)