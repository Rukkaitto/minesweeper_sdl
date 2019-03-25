project(MinesweeperText)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_library(game game.c)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMG REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} ${SDL2_IMG_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS} ${SDL2_IMG_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

include_directories(${SDL2_ALL_INC})
add_executable(minesweeper main.c minesweeper_sdl.c)
target_link_libraries(minesweeper ${SDL2_ALL_LIBS} m game)
