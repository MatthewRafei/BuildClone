cmake_minimum_required(VERSION 3.31.6)
project(BuildClone)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
set(SOURCE_FILES main.cpp)

add_executable(BuildClone ${SOURCE_FILES})
target_link_libraries(BuildClone SDL2::SDL2 SDL2::SDL2main) 
