cmake_minimum_required(VERSION 3.26)
project(ChessBot)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChessBot main.cpp
        chess.h
        chess.cpp
        bot.cpp
        path_node.cpp
        player.cpp)
