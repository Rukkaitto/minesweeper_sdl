project(MinesweeperText)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
add_executable(minesweeper_text minesweeper_text.c game.c)