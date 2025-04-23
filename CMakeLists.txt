cmake_minimum_required(VERSION 3.31.6)
project(BuildClone)
set(CMAKE_CXX_STANDARD 20)
find_package(SDL2 REQUIRED)

include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src/game
  ${CMAKE_SOURCE_DIR}/src/render
  ${CMAKE_SOURCE_DIR}/src/map
)


set(SOURCE_FILES
  src/main.cpp
  src/game/player.hpp
  src/map/map.hpp
  src/render/render.cpp
)

add_executable(BuildClone ${SOURCE_FILES})
target_link_libraries(BuildClone SDL2::SDL2 SDL2::SDL2main)
