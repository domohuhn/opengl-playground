cmake_minimum_required(VERSION 3.14)

project(glad VERSION 1.0.0)

add_library(glad
    src/gl.c
)

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
