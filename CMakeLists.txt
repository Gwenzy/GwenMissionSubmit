cmake_minimum_required(VERSION 3.13)
project(GwenMissionSubmit C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -lpthread")

add_executable(GwenMissionSubmit sources/main.c sources/num_methods.c headers/num_methods.h sources/html.c headers/html.h sources/struct_student.c headers/struct_student.h sources/display.c headers/display.h headers/constants.h)