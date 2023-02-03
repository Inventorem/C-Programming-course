cmake_minimum_required(VERSION 3.5.1)
project(lab4 C)

set(src src/main.c src/stack.h)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "../../Users/lev/Downloads/template-master/template-master/modules")
include(common_lab)
