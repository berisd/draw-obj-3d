cmake_minimum_required(VERSION 3.17)
project(draw_obj_3d C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)

add_executable(draw_obj_3d main.c render3d.c color.c video.c render.c)

target_link_libraries(draw_obj_3d ${SDL2_LIBRARIES} m)
