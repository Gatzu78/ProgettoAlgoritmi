cmake_minimum_required(VERSION 3.12)
project(ProgettoAlgoritmi C)

set(CMAKE_C_STANDARD 99)

add_executable(ProgettoAlgoritmi main.c lzss.c lzss.h aheadbuffer.c aheadbuffer.h utils.c utils.h AVL.h AVL.c datawriter.c datawriter.h)
