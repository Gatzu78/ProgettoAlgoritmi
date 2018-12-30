cmake_minimum_required(VERSION 3.12)
project(ProgettoAlgoritmi C)

set(CMAKE_C_STANDARD 99)

add_executable(ProgettoAlgoritmi main.c lzss.c lzss.h aheadbuffer.c aheadbuffer.h utils.c utils.h tree.c tree.h dictionary.h huffman_table.c huffman_table.h IOFile.c IOFile.h dictionary.c huffman.h huffman.c)
#add_executable(ProgettoAlgoritmi main.c lzss.c lzss.h huffman.c huffman.h stringtree.c stringtree.h AVL.c windowbuffer.c windowbuffer.h AVL.h aheadbuffer.c aheadbuffer.h utils.c utils.h tree.c tree.h dictionary.h huffman_table.c huffman_table.h IOFile.c IOFile.h dictionary.c)