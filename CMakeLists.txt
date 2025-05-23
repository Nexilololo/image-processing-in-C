cmake_minimum_required(VERSION 3.30)
project(image_processing_in_C C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(image_processing_in_C
    bmp8.c
    bmp8.h
    main.c
        bmp24.c)
